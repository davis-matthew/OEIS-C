/*
 * A243386  Number of length n+3 0..5 arrays with no four elements in a row
 *          with pattern abba (with a!=b) and new values 0..5 introduced in
 *          0..5 order.
 * https://oeis.org/A243386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 14, a(2) = 46, a(3) = 174, a(4) = 736, a(5) = 3406, a(6)
 *            = 16910, a(7) = 88473, a(8) = 480524, a(9) = 2677917, a(10) =
 *            15183903, a(11) = 87081621, a(12) = 503166559, a(13) =
 *            2921501848 a(n) = 16*a(n-1) - 99*a(n-2) + 313*a(n-3) -
 *            584*a(n-4) + 752*a(n-5) - 757*a(n-6) + 592*a(n-7) - 370*a(n-8...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* cache of computed terms, index 0 unused */
static size_t cache_len = 0;       /* highest index currently stored */

static const seq_t a_init[13] = {
    14ULL, 46ULL, 174ULL, 736ULL, 3406ULL,
    16910ULL, 88473ULL, 480524ULL, 2677917ULL,
    15183903ULL, 87081621ULL, 503166559ULL, 2921501848ULL
};

/* Return the nth term (n ≥ 1) of OEIS sequence A243386. */
seq_t a(idx_t n)
{
    /* For the first 13 terms just return the hard‑coded value. */
    if (n <= 13) return a_init[(size_t)(n - 1)];

    size_t nn = (size_t)n;

    /* Grow the cache if necessary. */
    if (nn >= cache_len) {
        size_t new_len = nn + 1;                /* index 0 unused */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
        a_cache = tmp;

        /* Initialise the first 13 cached terms if this is the first call. */
        if (cache_len == 0) {
            for (size_t i = 1; i <= 13; ++i)
                a_cache[i] = a_init[i - 1];
            cache_len = 13;
        }

        /* Compute any new terms using the recurrence. */
        for (size_t i = cache_len + 1; i <= nn; ++i) {
            seq_t t = 0, neg = 0;

            /* Positive contributions */
            t += a_cache[i - 1] * 16;
            t += a_cache[i - 3] * 313;
            t += a_cache[i - 5] * 752;
            t += a_cache[i - 7] * 592;
            t += a_cache[i - 9] * 191;
            t += a_cache[i - 11] * 25;
            t += a_cache[i - 13];

            /* Negative contributions */
            neg += a_cache[i - 2] * 99;
            neg += a_cache[i - 4] * 584;
            neg += a_cache[i - 6] * 757;
            neg += a_cache[i - 8] * 370;
            neg += a_cache[i - 10] * 74;
            neg += a_cache[i - 12] * 5;

            a_cache[i] = t - neg;
        }
        cache_len = nn;
    }

    return a_cache[n];
}
