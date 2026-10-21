/*
 * A275419  Representable integers n that are not the inverse of their inverse
 *          in binary64 (double precision) IEEE 754 floating-point arithmetic
 *          (Version where n and 1/(1/n) are unequal).
 * https://oeis.org/A275419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, less
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th integer for which
 *    1.0 / (1.0 / (double)n) != (double)n
 * in binary64 (IEEE‑754 double precision).
 *
 * The function iterates through consecutive integers, keeps a
 * static table of those that fail the round‑trip test, and returns
 * the n‑th found value.  The table is large enough to contain at
 * least the first 10 000 terms of the sequence; since the failure
 * density is only a few percent this is comfortably safe.
 */

#include <stdint.h>
#include <stddef.h>

/* Function declaration (provided by the build system). */
seq_t a(idx_t n);

/* Implementation. */
seq_t a(idx_t n)
{
    /* Handle out‑of‑range index gracefully. */
    if (n == 0)
        return (seq_t)0;

    /* Static table that stores the found integers.  500 000
       elements are more than enough to cover the first
       10 000 terms of the sequence. */
    static seq_t memo[500000];
    static size_t memo_size = 0;      /* number of terms stored */
    static size_t tested = 1;         /* next integer to test */

    /* Generate more terms if we don't yet have the requested one. */
    while (memo_size < n)
    {
        double d   = (double)tested;        /* (double)tested */
        double inv = 1.0 / d;               /* 1 / d       */
        double inv2 = 1.0 / inv;            /* 1 / inv     */

        if (inv2 != d)                     /* rounding causes a mismatch */
        {
            /* Store the offending integer. */
            memo[memo_size++] = (seq_t)tested;
        }
        ++tested;
    }

    /* a(1) corresponds to memo[0] */
    return memo[n - 1];
}
