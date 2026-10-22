/*
 * A307417  Numbers that can be expressed in a base in such a way that the sum
 *          of cubes of their digits in this base equals the original number.
 * https://oeis.org/A307417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 114, u32 n <= 114, u64 n <= 114,
 *            u128 n <= 114, big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A307417 – Numbers that can be expressed in some base b (b ≥ 2) 
 *  such that the sum of the cubes of their digits in that base equals
 *  the original number.
 *
 *  Algorithm: 
 *      For each candidate n>1 we check two cases
 *
 *      1.  Two‑digit representations:  n = a*b + c  with a,c < b,
 *          a^3 + c^3 = n.  We enumerate all a, compute c from n‑a^3,
 *          find base b = (n‑c)/a and verify b > max(a,c).
 *
 *      2.  Multi‑digit representations:  For each base b from 2 to ⌊√n⌋,
 *          compute the sum of cubes of the digits of n in base b.
 *          If the sum equals n we accept the number.
 *
 *      The sequence is generated incrementally; results are memoised
 *      so that subsequent calls can be answered in O(1) after the index
 *      has been built.
 *
 *  The program uses the helper functions _floor_isqrt_u64, floor_sqrt
 *  and floor_cube_root provided by the build environment.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

/* Test whether a given number n has a representation in some base b
 * such that the sum of the cubes of its digits equals n. */
static bool two_digit_candidate(seq_t n)
{
    /* upper bound on a is the integer cube root of n */
    seq_t max_a_seq = floor_cube_root(n);
    /* convert to a 64‑bit integer for iteration – the values we
     * need to examine are small for n up to the 10,000‑th term. */
    uint64_t max_a = (uint64_t)max_a_seq;
    for (uint64_t a = 1; a <= max_a; ++a) {
        seq_t a_seq = (seq_t)a;
        seq_t a3 = a_seq * a_seq * a_seq;
        if (a3 > n) break;
        seq_t rem = n - a3;
        seq_t b_seq = floor_cube_root(rem);
        seq_t b3 = b_seq * b_seq * b_seq;
        if (b3 != rem) continue;
        /* base from n = a*b + c :  b = (n – c) / a   (here b_seq == c) */
        seq_t numerator = n - b_seq;
        if (numerator % a_seq != 0) continue;
        seq_t base = numerator / a_seq;
        if (base <= a_seq || base <= b_seq || base < (seq_t)2) continue;
        return true;
    }
    return false;
}

static bool is_good(seq_t n)
{
    if (n < (seq_t)2) return false;

    /* Two‑digit case (covers bases both ≤√n and >√n). */
    if (two_digit_candidate(n)) return true;

    /* Multi‑digit representations require base ≤ √n. */
    seq_t sqrt_seq = floor_sqrt(n);
    uint64_t sqrt_lim = (uint64_t)sqrt_seq;
    for (uint64_t base = 2; base <= sqrt_lim; ++base) {
        seq_t base_seq = (seq_t)base;
        seq_t temp = n;
        seq_t sum = (seq_t)0;
        while (temp) {
            seq_t digit = temp % base_seq;
            seq_t digit3 = digit * digit * digit;
            sum += digit3;
            if (sum > n) break;
            temp /= base_seq;
        }
        if (sum == n) return true;
    }
    return false;
}

/*------------------------------------------------------------------*/
/*  Public interface required by the judge.                         */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *terms = NULL;
    static size_t sz = 0;
    static size_t cap = 0;

    static seq_t next_candidate = (seq_t)1;   /* start from 1 */

    if (terms == NULL) {
        cap = 16;
        terms = (seq_t *)malloc(cap * sizeof(*terms));
        if (!terms) exit(1);
    }

    while (sz < (size_t)n) {
        if (is_good(next_candidate)) {
            if (sz >= cap) {
                cap <<= 1;
                terms = (seq_t *)realloc(terms, cap * sizeof(*terms));
                if (!terms) exit(1);
            }
            terms[sz++] = next_candidate;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
