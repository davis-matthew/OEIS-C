/*
 * A277255  Let A_n defined in the same way as A159559, but with initial term
 *          prime(n); a(n) is the smallest number m such that A_(n+1)(m) -
 *          A_n(m) > 6 or a(n)=0 if there is no such m.
 * https://oeis.org/A277255
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   search. A_n is A159559's rule (a(2)=prime(n); each later term is
 *            the smallest larger number that is prime iff its index is
 *            prime). a(n) = least m>=2 with A_{n+1}(m)-A_n(m) > 6, else 0.
 *            The search stops at a fixed index limit
 * Range:     u8 n <= 5001, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Validated: standard, pass, 99 terms through n = 4499 (2026-11-08)
 * Helpers:   is_prime_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/nth_prime.h"

#define A277255_M 4000

static bool A277255_idxp[A277255_M + 1];
static bool A277255_init = false;

static void A277255_setup(void)
{
    if (A277255_init) return;
    for (int i = 0; i <= A277255_M; i++) A277255_idxp[i] = (i >= 2);
    for (int i = 2; (long)i * i <= A277255_M; i++)
        if (A277255_idxp[i])
            for (int j = i * i; j <= A277255_M; j += i) A277255_idxp[j] = false;
    A277255_init = true;
}

static uint64_t A277255_step(uint64_t prev, bool need_prime)
{
    uint64_t c = prev + 1;
    if (need_prime) {
        while (!is_prime_u64(c)) c++;
    } else {
        while (is_prime_u64(c)) c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    A277255_setup();
    uint64_t x = nth_prime((idx_t)n);
    uint64_t y = nth_prime((idx_t)(n + 1));
    if (y > x + 6) return (seq_t)2;
    for (int m = 3; m <= A277255_M; m++) {
        bool ip = A277255_idxp[m];
        x = A277255_step(x, ip);
        y = A277255_step(y, ip);
        if (y > x + 6) return (seq_t)m;
    }
    return (seq_t)0;
}
