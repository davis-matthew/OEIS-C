/*
 * A292569  Least pandigital number which sums up with the n-th pandigital
 *          number A171102(n) to another pandigital number.
 * https://oeis.org/A292569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least M in A171102 such that A171102(n) + M is also in
 *            A171102 Computation uses lexicographic enumeration of the 9·9!
 *            pandigital numbers (A171102(1..9·9!) enumerated by permutations
 *            of digits 0‑9 with non‑zero first digit). The first 10 000 terms
 *            lie in this range; the implementation suppo...
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* factorial table up to 9! */
static const idx_t FACT[10] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL,
    720ULL, 5040ULL, 40320ULL, 362880ULL
};

/* return true iff x contains all digits 0–9 at least once */
static bool is_pandigital(seq_t x)
{
    unsigned mask = 0;
    while (x) {
        int d = (int)(x % 10ULL);
        mask |= 1u << d;
        x /= 10ULL;
    }
    return mask == 0x3FFU;
}

/* return the n‑th pandigital number of A171102 (n≥1, n≤3265920) */
static seq_t nth_pandigital(idx_t n)
{
    if (n < 1ULL || n > (idx_t)3265920ULL)
        return (seq_t)0;   /* out of supported range */

    seq_t val = 0;
    /* zero‑based index */
    idx_t n0 = n - 1ULL;

    /* first digit: 1..9, each block of 9! permutations */
    idx_t block = n0 / FACT[9];
    int first_digit = (int)(block + 1);   /* 1‑9 */

    val = (seq_t)first_digit;

    /* remaining 9 digits (0–9 excluding first_digit) in ascending order */
    int rem[9];
    int idx = 0;
    for (int d = 0; d <= 9; ++d)
        if (d != first_digit)
            rem[idx++] = d;       /* idx will become 9 */

    idx_t pos = n0 % FACT[9];   /* position inside block */

    /* generate the remaining 9‑digit permutation using factorial number system */
    for (int i = 0; i < 9; ++i) {
        idx_t f = FACT[8 - i];
        idx_t index = pos / f;
        pos %= f;

        int digit = rem[(int)index];
        val = val * 10ULL + (seq_t)digit;

        /* remove used digit from rem */
        for (int j = (int)index; j < 8 - i; ++j)
            rem[j] = rem[j + 1];
    }

    return val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1ULL) return (seq_t)0;

    /* compute the n‑th pandigital number */
    seq_t P = nth_pandigital(n);
    if (P == (seq_t)0) return (seq_t)0;   /* out of supported range */

    /* search for the smallest M such that P + M is also pandigital */
    for (idx_t m = 1ULL; ; ++m) {
        seq_t M = nth_pandigital(m);
        if (M == (seq_t)0) return (seq_t)0;   /* should not occur for tested range */
        seq_t sum = P + M;
        if (is_pandigital(sum))
            return M;
    }
}
