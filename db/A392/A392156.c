/*
 * A392156  a(n) is the number of distinct primes obtainable by inserting
 *          exactly one decimal digit into the decimal representation of
 *          prime(n) (at any position, including before the first digit or
 *          after the last), with the resulting number required to have no
 *          leading zero.
 * https://oeis.org/A392156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1907, u32 n <= 1907, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, digit_len, get_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements A392156:  a(n) = number of distinct primes obtained by
   inserting one decimal digit into the nth prime (prime(n)), with no
   leading zero in the resulting number. */
#include <stdbool.h>
#include <stdlib.h>

/* utilities required for the implementation */
#include "../utils/get_prime.h"        /* get_prime(idx_t) -> uint32_t */
#include "../utils/digit_len.h"       /* digit_len(seq_t) -> int */
#include "../utils/pow10.h"           /* pow10(idx_t) -> seq_t */
#include "../utils/is_prime_big.h"    /* is_prime_big(seq_t) -> bool */
#include "../utils/cmp_seq_t.h"       /* cmp_seq_t(const void*, const void*) */

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset is 1; behaviour below 1 irrelevant */

    /* ---------- 1. obtain the nth prime --------------------------------- */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;          /* promote to seq_t */

    /* ---------- 2. number of decimal digits of p ----------------------- */
    int L_int = digit_len(p);
    if (L_int < 0) return 0;          /* safety: should not happen */
    uint32_t L = (uint32_t)L_int;     /* convert to unsigned for loops */

    /* ---------- 3. pre‑compute powers of 10: 10^0 … 10^L ---------------- */
    seq_t pow10[21];                  /* L <= 6 for n <= 10000; 21 is safe */
    pow10[0] = (seq_t)1;
    for (uint32_t i = 1; i <= L; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    /* ---------- 4. generate all candidate primes ------------------------- */
    /* maximum candidates = (L+1) * 10; safe upper bound: 70 for L=6        */
    seq_t cand[100];
    size_t cand_cnt = 0;

    for (uint32_t pos = 0; pos <= L; ++pos) {
        /* pow10_exp = 10^(L-pos) */
        uint32_t exp = L - pos;
        seq_t pow = pow10[exp];

        /* left  = floor(p / pow), right = p % pow          */
        seq_t left  = p / pow;
        seq_t right = p % pow;

        for (uint8_t d = 0; d <= 9; ++d) {
            if (pos == 0 && d == 0) continue;    /* no leading zero */

            /* newnum = (left*10 + d)*pow + right */
            seq_t newnum = ((left * (seq_t)10) + (seq_t)d) * pow + right;

            if (is_prime_big(newnum)) {
                cand[cand_cnt++] = newnum;
            }
        }
    }

    if (cand_cnt == 0) return 0;

    /* ---------- 5. remove duplicates ------------------------------------- */
    qsort(cand, cand_cnt, sizeof(seq_t), cmp_seq_t);

    size_t unique = 1;
    for (size_t i = 1; i < cand_cnt; ++i)
        if (cand[i] != cand[i - 1])
            ++unique;

    return (seq_t)unique;
}
