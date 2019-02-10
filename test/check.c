#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "util.h"
#include "fon.h"

Suite* lSocket_suite (void);
lSocket S;
int numSocket1;
int numSocket2;

int main () {
  int number_failed;
  SRunner *sr;

  sr = srunner_create(lSocket_suite());
  //srunner_add_suite (sr, Tas_suite());

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void setup (void) {
  makeLSocket(&S);

  numSocket1 = h_socket(AF_INET, SOCK_STREAM); /* création de la socket */
  numSocket2 = h_socket(AF_INET, SOCK_STREAM); /* création de la socket */
  setSocket(&S, numSocket1);
  setSocket(&S, numSocket2);
}
void teardown (void) {
  rmLSocket(&S);
}

START_TEST (test_length)
{
  ck_assert_int_eq(getLength(S), 2);
  rmSocket(&S, numSocket2);
  ck_assert_int_eq(getLength(S), 1);
}
END_TEST

START_TEST (test_socket)
{
  int numSocket3 = h_socket(AF_INET, SOCK_STREAM); /* création de la socket */
  setSocket(&S, numSocket3);
  ck_assert_int_eq(getSocket(S, 3), numSocket3);
}
END_TEST

START_TEST (test_socket_fail)
{
  ck_assert_int_eq(getSocket(S, 4), 4);
}
END_TEST

Suite* lSocket_suite (void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("liste chainee Socket");

  /* Card */
  tc_core = tcase_create("basis methods");
  tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_length);
  tcase_add_test(tc_core, test_socket);
  tcase_add_exit_test(tc_core, test_socket_fail, 1);
  suite_add_tcase(s, tc_core);

  return s;
}
