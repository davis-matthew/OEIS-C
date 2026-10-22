/*
 * A379271  Composite numbers, k, whose prime factors, viewed on a log log
 *          scale, have a small standard deviation defined with respect to
 *          bigomega(k), as specified in the comments.
 * https://oeis.org/A379271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the composite integers k that satisfy * var( {
 *            log(log(p_i)) } ) < (m^2‑1)/12 , * where m = bigomega(k) (count
 *            with multiplicity). * The program computes the condition exactly
 *            using a fixed‑point * representation of log(log(p)) with scaling
 *            factor 1e9; all * arithmetic is performed with...
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* static global data ----------------------------------------------------- */
static uint32_t *spf            = NULL;  /* smallest prime factor (up to limit) */
static int64_t  *loglog_scaled  = NULL;  /* log(log(p)) scaled by 1e9          */
static seq_t    *qualifiers     = NULL;  /* array of qualifying numbers         */
static size_t   qual_size       = 0;     /* number of terms stored so far      */
static size_t   qual_cap        = 0;     /* capacity of qualifiers array        */
static uint32_t cur_limit       = 0;     /* current sieved upper bound          */
#define SCALE 1000000000LL           /* fixed‑point scaling factor          */

/* ------------------------------------------------------------------------ */
/* Rebuild all lookup tables up to NEW_LIMIT.                                 */
static void recompute_up_to(uint32_t new_limit)
{
    free(spf);
    free(loglog_scaled);
    free(qualifiers);

    cur_limit = new_limit;

    spf            = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
    loglog_scaled  = (int64_t*)malloc((new_limit + 1) * sizeof(int64_t));
    qual_cap       = (new_limit / 2 + 1024);
    qualifiers     = (seq_t*)malloc(qual_cap * sizeof(seq_t));
    qual_size      = 0;

    /* smallest prime factor sieve ---------------------------------------- */
    for (uint32_t i = 0; i <= new_limit; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= new_limit) {
                for (uint32_t j = i * i; j <= new_limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* pre‑compute scaled log(log(p)) for primes -------------------------- */
    loglog_scaled[0] = loglog_scaled[1] = 0;
    for (uint32_t p = 2; p <= new_limit; ++p) {
        if (spf[p] == p) {          /* p is prime */
            double v = log(log((double)p));
            loglog_scaled[p] = (int64_t)(v * SCALE + 0.5);
        } else {
            loglog_scaled[p] = 0;    /* unused for composites */
        }
    }

    /* build list of qualifying composite numbers -------------------------- */
    for (uint32_t n = 4; n <= new_limit; ++n) {
        uint32_t temp = n;
        int m = 0;
        int64_t sumX = 0;
        unsigned __int128 sumSq = 0;

        while (temp > 1) {
            uint32_t prime = spf[temp];
            int64_t val = loglog_scaled[prime];
            sumX += val;
            __int128 vv = (__int128)val;
            sumSq += (unsigned __int128)(vv * vv);
            ++m;
            temp /= prime;
        }
        if (m <= 1) continue;   /* not a composite with at least two prime factors */

        /* left = m*sumSq - sumX^2  (all 128‑bit) */
        unsigned __int128 left = (unsigned __int128)m * sumSq -
                                 (unsigned __int128)((__int128)sumX * (__int128)sumX);

        /* right = ((m^2-1) * m^2 * SCALE^2) / 12  (128‑bit) */
        uint64_t mm = (uint64_t)m;
        unsigned __int128 right = (unsigned __int128)(mm * mm - 1) *
                                (unsigned __int128)(mm * mm);
        right *= (unsigned __int128)SCALE * (unsigned __int128)SCALE;
        right /= 12u;

        if (left < right) {
            if (qual_size == qual_cap) {
                qual_cap *= 2;
                qualifiers = (seq_t*)realloc(qualifiers, qual_cap * sizeof(seq_t));
            }
            qualifiers[qual_size++] = (seq_t)n;
        }
    }
}

/* ------------------------------------------------------------------------ */
/* Ensure that the cache contains at least N qualifiers.                    */
static void ensure_sequence_for_index(idx_t n)
{
    if (n == 0) return;
    while (qual_size < n) {
        uint32_t new_limit = cur_limit == 0 ? 100 : cur_limit * 2 + 1;
        recompute_up_to(new_limit);
    }
}

/* ------------------------------------------------------------------------ */
/* Public OEIS function.                                                    */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for index 0 */
    ensure_sequence_for_index(n);
    return qualifiers[n - 1];
}
