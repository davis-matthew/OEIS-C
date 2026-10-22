/*
 * A359769  a(n) = A353557(n) - A353556(n).
 * https://oeis.org/A359769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n is odd and Ω(n) is even; * a(n) = -1 if n is even
 *            and Ω(n) is odd; * a(n) = 0 otherwise. * Ω(n) is the total
 *            number of prime factors of n with multiplicity. * The
 *            implementation builds a smallest‑prime‑factor table up to the *
 *            largest n requested (lazy construction, works for any ca...
 * Range:     i8 n <= 8238, i16 n <= 8238, i32 n <= 8238, i64 n <= 8238,
 *            i128 n <= 8238, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t *spf = NULL;          /* smallest prime factor array (1‑based) */
static idx_t spf_cap = 0;          /* current capacity of spf */

/* Rebuild spf array up to at least ‘limit’.  The whole table is rebuilt
 * because calls to a() are sparse and rebuilding only when a larger
 * limit is seen keeps the number of rebuilds logarithmic in the max n. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_cap) return;
    free(spf);
    spf = (idx_t *)calloc(limit + 1, sizeof(idx_t));
    if (!spf) return;  /* allocation failure – unlikely in normal tests */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {                /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }
    spf_cap = limit;
}

/* Return the parity (0 even, 1 odd) of Ω(x) for an odd integer x
 * (x ≥ 1).  The function assumes spf is built up to x. */
static int omega_parity_odd(idx_t x)
{
    int parity = 0;
    while (x > 1) {
        idx_t p = spf[x];
        if (!p) p = x;               /* x is prime */
        while ((x % p) == 0) {
            x /= p;
            parity ^= 1;             /* flip parity for each prime factor */
        }
    }
    return parity;                   /* 0 for even Ω(x), 1 for odd Ω(x) */
}

/* Main entry point for OEIS sequence A359769. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined offset – safe fallback */

    /* Count exponent of 2 in n.  m is the odd part. */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }

    /* Build spf up to the odd part if necessary.  For n == 1, m == 1 gives
     * omega_parity 0, which is fine. */
    if (m > 1) ensure_spf(m);

    /* Parity of Ω(m) (m is odd). */
    int parity_s = omega_parity_odd(m);

    int p = (e + parity_s) & 1;      /* parity of e + Ω(m) */

    if (e == 0) {                    /* n is odd */
        return (p == 0) ? (seq_t)1 : (seq_t)0;
    } else {                          /* n is even */
        return (p == 1) ? (seq_t)-1 : (seq_t)0;
    }
}
