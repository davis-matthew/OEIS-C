/*
 * A309027  Prime powers of the form 12*c^2 + 4*c + 3, where c is an arbitrary
 *          integer.
 * https://oeis.org/A309027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309027: Prime powers of 12*c^2 + 4*c + 3, where c is an integer.
   The terms are obtained by enumerating c as 0, -1, 1, -2, 2, -3, 3, …,
   testing each value for being a prime power.
   For the ranges considered all such values are (conjecturally) prime,
   so the code tests primality only. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return the n-th term, with OEIS offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return safe value */
        return (seq_t)0;
    if (n == 1)          /* first term is 3, when c = 0 */
        return (seq_t)3;

    idx_t count = 1;     /* we already have the first prime 3 */
    idx_t k = 1;         /* start with |c| = 1 */

    while (1) {
        seq_t kt = (seq_t)k;

        /* c = -k : 12*k^2 - 4*k + 3 */
        seq_t val_neg = ((seq_t)12) * kt * kt - ((seq_t)4) * kt + ((seq_t)3);
        if (is_prime_big(val_neg)) {
            ++count;
            if (count == n)
                return val_neg;
        }

        /* c = +k : 12*k^2 + 4*k + 3 */
        seq_t val_pos = ((seq_t)12) * kt * kt + ((seq_t)4) * kt + ((seq_t)3);
        if (is_prime_big(val_pos)) {
            ++count;
            if (count == n)
                return val_pos;
        }

        ++k;
    }
}
