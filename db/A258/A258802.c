/*
 * A258802  Least base b >= 2 such that prime(n) is an absolute prime in base
 *          b with at least 2 distinct digits or 0 if no such base exists.
 * https://oeis.org/A258802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1011, u32 n <= 1011, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A258802:  least base b ≥ 2 such that
   prime(n) is an absolute prime in base b with at least two
   distinct digits, or 0 if no such base exists. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Recursively test every distinct permutation of the multiset of digits.
   'base' is the radix, 'vals' and 'cnts' contain the distinct digit values
   and their remaining counts, 'num_vals' the number of distinct digits.
   'pos' is the number of remaining positions to fill, 'acc' is the
   value already accumulated for the prefix.  Returns true if all
   permutations form a prime number; false as soon as a single
   non‑prime permutation is found. */
static bool test_permutations(idx_t base,
                              unsigned int *vals, const size_t *cnts_orig,
                              size_t num_vals, size_t pos, seq_t acc)
{
    /* local copy of counts because we modify them */
    size_t cnts[64];
    for (size_t i = 0; i < num_vals; ++i)
        cnts[i] = cnts_orig[i];

    if (pos == 0)
        return is_prime_big(acc);

    for (size_t i = 0; i < num_vals; ++i)
    {
        if (cnts[i] == 0)
            continue;
        cnts[i]--;
        seq_t new_acc = acc * (seq_t)base + (seq_t)vals[i];
        if (!test_permutations(base, vals, cnts, num_vals, pos - 1, new_acc))
        {
            cnts[i]++;   /* restore and abort */
            return false;
        }
        cnts[i]++;       /* backtrack */
    }
    return true; /* should never reach when pos > 0 */
}

/* Test whether 'p' (prime) is an absolute prime in base 'b'.
   The representation must contain at least two distinct digits
   and no zero digit. */
static bool is_absolute_prime_base(seq_t p, idx_t b)
{
    unsigned int digits[64];
    size_t len = 0;
    seq_t tmp = p;

    /* Extract digits of p in base b (least‑significant first). */
    while (tmp > 0)
    {
        digits[len++] = (unsigned int)(tmp % (seq_t)b);
        tmp /= (seq_t)b;
    }
    if (len < 2)
        return false;                     /* need at least two digits */

    /* Check for zero digit and collect distinct digits. */
    unsigned int distinct[64];
    size_t distinct_cnt = 0;
    for (size_t i = 0; i < len; ++i)
    {
        unsigned int d = digits[i];
        if (d == 0)
            return false;                 /* zero invalidates absolute prime */

        bool seen = false;
        for (size_t j = 0; j < distinct_cnt; ++j)
            if (distinct[j] == d) { seen = true; break; }

        if (!seen)
            distinct[distinct_cnt++] = d;
    }
    if (distinct_cnt < 2)
        return false;                     /* need at least two distinct digits */

    /* Build arrays of unique digits and their multiplicities. */
    unsigned int vals[64];
    size_t cnts[64];
    size_t vcnt = 0;
    for (size_t i = 0; i < len; ++i)
    {
        unsigned int d = digits[i];
        bool found = false;
        for (size_t j = 0; j < vcnt; ++j)
        {
            if (vals[j] == d) { cnts[j]++; found = true; break; }
        }
        if (!found)
        {
            vals[vcnt] = d;
            cnts[vcnt] = 1;
            vcnt++;
        }
    }

    /* Recursively test all distinct permutations. */
    return test_permutations(b, vals, cnts, vcnt, len, 0);
}

/* OEIS sequence A258802.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n <= 2)         /* the first two terms are known to be 0 */
        return 0;

    /* obtain the n‑th prime (returns unsigned int, cast to seq_t). */
    seq_t p = (seq_t)get_prime(n);

    /* search for the smallest base b ≥ 2. */
    for (idx_t b = 2; b < (idx_t)p; ++b)
        if (is_absolute_prime_base(p, b))
            return (seq_t)b;

    return 0;           /* none found */
}
