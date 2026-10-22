/*
 * A354023  Terms m of A351467 such that A005245(m) == 0 (mod 3).
 * https://oeis.org/A354023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   exponent_of_power_of_three, exponent_of_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation of OEIS A354023.
 *  The sequence consists of all integers m>1 that are
 *  members of A351467 and for which A005245(m) ≡ 0 (mod 3).
 *
 *  A number m qualifies iff it can be written as 
 *      2^p * 3^r,          p ∈ {0,3,6,9},
 *      2*(3^r + 1)*3^s,    r > 0,
 *      (2*3^r + 1)*3^s,    r ≥ 0.
 *
 *  This implementation generates the terms in increasing order
 *  by testing each integer m ≥ 3 for the three possible forms.
 *  The results are memoised in a static array so that subsequent
 *  calls with any index are answered in constant time.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include "../utils/exponent_of_power_of_three.h"
#include "../utils/exponent_of_power_of_two.h"

/* ------------------------------------------------------------------
 * Test whether m belongs to the sequence.
 * ------------------------------------------------------------------ */
static bool is_member(seq_t m)
{
    /* 1. m = 2^p * 3^r   with p ∈ {0,3,6,9} */
    seq_t t = m;
    while (t % 3 == 0) t = t / 3;          /* remove all 3's */
    int p = exponent_of_power_of_two(t);
    if (p >= 0 && (p == 0 || p == 3 || p == 6 || p == 9))
        return true;

    /* 2. m = 2*(3^r + 1)*3^s   (r > 0) */
    if (m % 2 == 0) {
        seq_t u = m / 2;
        while (u % 3 == 0) u = u / 3;      /* u = 3^r + 1 */
        seq_t v = u - 1;
        int r = exponent_of_power_of_three(v);
        if (r > 0)
            return true;
    }

    /* 3. m = (2*3^r + 1)*3^s   (r ≥ 0) */
    seq_t t2 = m;
    while (t2 % 3 == 0) t2 = t2 / 3;        /* remove all 3's */
    if ((t2 - 1) % 2 == 0) {
        seq_t w = (t2 - 1) / 2;             /* w = 3^r */
        int r = exponent_of_power_of_three(w);
        if (r >= 0)
            return true;
    }

    return false;
}

/* ------------------------------------------------------------------
 * Memoised table of terms.
 * The array size must accommodate the maximum n expected by the
 * tests (here we use a generous limit of 32768 terms).
 * ------------------------------------------------------------------ */
#define MAX_TERMS 32768
static seq_t table[MAX_TERMS];
static size_t len = 0;
static seq_t next_test = 3;   /* first number to test */

/* ------------------------------------------------------------------
 * Main sequence function required by the evaluation framework.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure we have generated at least n terms. */
    if (len < (size_t)n) {
        seq_t m = next_test;
        /* Generate until we accumulate n terms. */
        while (len < (size_t)n) {
            if (is_member(m))
                table[len++] = m;
            m = m + 1;          /* auto‑extension for seq_t */
        }
        next_test = m;          /* next number to test on subsequent calls */
    }
    return table[n - 1];
}
