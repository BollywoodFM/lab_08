#include "Account.h"
#include "Transaction.h"
#include <stdexcept>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class Acc : public Account {
 private:
  int id;
  int balance;
 public:
  Acc(int id, int balance) : Account(id, balance) {}
  MOCK_METHOD(int, GetBalance, (), (const, override));
  MOCK_METHOD(void, ChangeBalance, (int), (override));
  MOCK_METHOD(void, Lock, (), (override));
  MOCK_METHOD(void, Unlock, (), (override));
};

class Tra : public Transaction {
 public:
  Tra() : Transaction() {}
  MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

using ::testing::AtLeast;

TEST(Account, start) {
  Acc q1(1, 1000);
  EXPECT_CALL(q1, GetBalance()).Times(AtLeast(1));
  q1.GetBalance();
  EXPECT_CALL(q1, Lock()).Times(AtLeast(1));
  q1.Lock();
  EXPECT_CALL(q1, ChangeBalance(1000)).Times(AtLeast(1));
  q1.ChangeBalance(1000);
  EXPECT_CALL(q1, Unlock()).Times(AtLeast(1));
  q1.Unlock(); 
}

TEST(Transaction, start) {
  Account q1(1, 10000);
  Account q2(2, 10000);
  Tra t1;
  EXPECT_CALL(t1, SaveToDataBase(q1, q2, 1999)).Times(AtLeast(1));
  t1.Make(q1, q2, 1999);
}

TEST(Account, main) {
  Account ac1(1, 1000);
  EXPECT_EQ(1000, ac1.GetBalance());
  ac1.Lock();
  ac1.ChangeBalance(2000);
  ac1.Unlock();
  EXPECT_EQ(3000, ac1.GetBalance());
  try {
    ac1.ChangeBalance(1);
  }
  catch (std::runtime_error& el) {}
  EXPECT_EQ(3000, ac1.GetBalance());
}

TEST(Transaction, main) {
  Account ac1(1, 10000);
  Account ac2(2, 10000);
  Transaction t1;
  Transaction t2; t2.set_fee(500);
  try {t1.Make(ac1, ac1, 100);}
  catch (std::logic_error& el) {}
  try {t1.Make(ac1, ac2, -100); }
  catch (std::invalid_argument& el) {}
  try {t1.Make(ac1, ac2, 0);}
  catch (std::logic_error& el) {}
  EXPECT_EQ(false, t2.Make(ac1, ac2, 200));
  t1.Make(ac1, ac2, 20000);
  EXPECT_EQ(ac1.GetBalance(), 10000); EXPECT_EQ(ac2.GetBalance(), 9999);
}