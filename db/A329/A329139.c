/*
 * A329139  Numbers whose prime signature is an aperiodic word.
 * https://oeis.org/A329139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   nth element computed by brute‑force search of numbers whose
 *            prime signature is an aperiodic word
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Factor n and return the exponents of its prime factors in exps[]. */
static int factor_exponents(uint64_t n, int exps[])
{
    int k = 0;
    int cnt = 0;

    /* factor powers of 2 */
    while ((n & 1) == 0) {
        n >>= 1;
        ++cnt;
    }
    if (cnt > 0)
        exps[k++] = cnt;

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        cnt = 0;
        while (n % p == 0) {
            n /= p;
            ++cnt;
        }
        if (cnt > 0)
            exps[k++] = cnt;
    }

    /* any remaining prime factor > sqrt(n) */
    if (n > 1)
        exps[k++] = 1;
    return k;
}

static bool is_aperiodic(const int exps[], int k)
{
    if (k <= 1)
        return true; /* length 0 or 1 is vacuously aperiodic */
    for (int shift = 1; shift < k; ++shift) {
        bool eq = true;
        for (int i = 0; i < k; ++i) {
            if (exps[i] != exps[(i + shift) % k]) {
                eq = false;
                break;
            }
        }
        if (eq)
            return false; /* periodic */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but avoid underflow */

    uint64_t target = (uint64_t)n;
    uint64_t count  = 0;
    uint64_t m      = 1;

    const int MAX_EXP = 64; /* enough for any 64‑bit integer */
    int exps[MAX_EXP];

    for (;; ++m) {
        int k = factor_exponents(m, exps);
        if (is_aperiodic(exps, k)) {
            ++count;
            if (count == target)
                return (seq_t)m;
        }
    }
}
