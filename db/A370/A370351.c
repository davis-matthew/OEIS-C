/*
 * A370351  The smallest number such that n or more numbers k exist such that
 *          k - a(n) = sopfr(k + a(n)), where sopfr(m) is the sum of the
 *          primes dividing m, with repetition.
 * https://oeis.org/A370351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the smallest number a such that there exist at least n
 *            integers k with k - a = sopfr(k + a) (sopfr(m) = sum of the
 *            prime divisors of m, counted with multiplicities)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* All types and helper headers are supplied by the build system.  We
 * include <stdbool.h> so we can use bool.  We rely on the build system
 * to provide the following types. */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Smallest prime factor sieve for numbers up to a fixed limit.  The
 * limit is chosen large enough to handle the range of m we need for
 * n up to about 10 000.  It is grown only once, at first use. */
static uint32_t *spf = NULL;
static size_t   spf_limit = 0;

static void init_spf(void)
{
    if (spf != NULL) return;

    /* 20 million is enough for the tested range; it uses ~80 MB of memory. */
    const idx_t LIMIT = 20000000U;        /* 20 million */
    spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!spf) abort();
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }
    spf_limit = LIMIT;
}

/* ------------------------------------------------------------------ */
/* Sum of prime factors with multiplicity (sopfr) for an unsigned 64‑bit
 * integer.  Uses the pre‑computed sieve when possible, otherwise falls
 * back to trial division. */
static uint64_t sopfr_u64(uint64_t n)
{
    uint64_t sum = 0;
    while (n > 1) {
        uint32_t p;
        if (n <= spf_limit) {
            p = spf[n];
        } else {
            /* Very rarely reached for the required range. */
            p = (uint32_t)n;
            for (uint64_t q = 2; q * q <= n; ++q) {
                if (n % q == 0) { p = (uint32_t)q; break; }
            }
        }
        sum += p;
        n /= p;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Counts of how many k have produced a particular a value.  We store
 * them in a dynamic array indexed by a (1‑based).  We use uint32_t to
 * be safe for large n. */
static uint32_t *counts = NULL;
static idx_t    counts_alloc = 0;   /* allocated size (indexable up to counts_alloc‑1) */

static void ensure_counts_capacity(idx_t a)
{
    if (a < counts_alloc) return;
    idx_t new_cap = counts_alloc ? counts_alloc << 1 : 16;
    while (new_cap <= a) new_cap <<= 1;
    uint32_t *new_arr = (uint32_t *)realloc(counts, new_cap * sizeof(uint32_t));
    if (!new_arr) abort();
    /* Initialize new part to 0 */
    memset(new_arr + counts_alloc, 0, (new_cap - counts_alloc) * sizeof(uint32_t));
    counts = new_arr;
    counts_alloc = new_cap;
}

/* ------------------------------------------------------------------ */
/* best[c] holds the smallest a that achieves at least c solutions.
 * best[0] is unused.  best_filled is the largest index with a known
 * value. */
static seq_t *best = NULL;          /* best[c] = minimal a achieving at least c solutions */
static idx_t   best_alloc = 0;       /* allocated size of best array */
static idx_t   best_filled = 0;     /* largest index with a known answer (>=1) */

static void ensure_best_capacity(idx_t need)
{
    if (need <= best_alloc) return;
    idx_t new_cap = best_alloc ? best_alloc << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(best, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    /* Initialize new part to 0 (the zero element of seq_t). */
    memset(new_arr + best_alloc, 0, (new_cap - best_alloc) * sizeof(seq_t));
    best = new_arr;
    best_alloc = new_cap;
}

/* ------------------------------------------------------------------ */
/* Global state: the last m processed.  We start at m = 1. */
static uint64_t last_m = 1;

/* ------------------------------------------------------------------ */
/* Process m values until we have computed at least best_filled >= target.
 * For each m, compute diff = m - sopfr(m).  If diff is positive and
 * even, we have a candidate a = diff/2.  Increment the count for this
 * a and possibly update the best array. */
static void process_until(idx_t target)
{
    init_spf();                     /* ensure spf sieve is ready */

    while (best_filled < target) {
        ++last_m;
        uint64_t sop = sopfr_u64(last_m);
        int64_t diff = (int64_t)last_m - (int64_t)sop;
        if (diff <= 0 || (diff & 1LL)) continue;   /* a must be a positive integer */

        idx_t a_val = (idx_t)(diff >> 1);
        if (a_val == 0) continue;                /* skip zero (shouldn't happen) */

        ensure_counts_capacity(a_val);
        uint32_t old_c = counts[a_val];
        uint32_t new_c = old_c + 1;
        counts[a_val] = new_c;

        ensure_best_capacity((idx_t)new_c);
        if (best[new_c] == 0 || (seq_t)a_val < best[new_c]) best[new_c] = (seq_t)a_val;

        /* Advance best_filled while the next entries are set. */
        while (best_filled + 1 < best_alloc && best[best_filled + 1] != 0)
            ++best_filled;
    }
}

/* ------------------------------------------------------------------ */
/* The required sequence function.  It is memoised; calls may arrive in
 * any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, return 0 as a placeholder */
    process_until(n);
    return best[n];
}
