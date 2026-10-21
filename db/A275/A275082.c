/*
 * A275082  Primitive abundant numbers (having no abundant proper divisors)
 *          that have perfect proper divisors.
 * https://oeis.org/A275082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1694, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A275082 – Primitive abundant numbers that have a perfect proper divisor.
    The terms are the numbers of the form
          2·N  and  N·q ,
    where N is an even perfect number (6, 28, 496, 8128 …) and q is a prime
    greater than or equal to the Mersenne prime that appears in the factorisation
    of N.  The function generates these streams, merges them in increasing
    order, and returns the n‑th term (a(1)=12).  The algorithm is O(n).  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime sieve ---------- */

static bool primes_inited = false;
static idx_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_inited)
        return;

    /* limit chosen to supply enough primes for n up to 10 000. */
    const size_t LIMIT = 200000;
    uint8_t *is_composite = (uint8_t *)calloc(LIMIT + 1, 1);
    if (!is_composite)
        exit(EXIT_FAILURE);

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_composite[j] = 1;

    /* count primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i])
            ++primes_cnt;

    primes = (idx_t *)malloc(primes_cnt * sizeof(idx_t));
    if (!primes)
        exit(EXIT_FAILURE);

    size_t pos = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i])
            primes[pos++] = (idx_t)i;

    free(is_composite);
    primes_inited = true;
}

/* ---------- binary search helper ---------- */

/* first index where arr[idx] >= val */
static size_t lower_bound(const idx_t *arr, size_t n, idx_t val)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;      /* may be n */
}

/* ---------- stream of multiples of a perfect number ---------- */

struct Stream {
    seq_t base;          /* even perfect number N */
    size_t p_idx;        /* index of current prime ≥ threshold */
    bool used_two;       /* whether 2·N has already been output */
};

/* ---------- main sequence generator ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined but harmless */

    init_primes();

    /* first four known even perfect numbers and their prime thresholds */
    static const seq_t perfect_N[] = { (seq_t)6ULL, (seq_t)28ULL,
                                       (seq_t)496ULL, (seq_t)8128ULL };
    static const idx_t  threshold_q[] = { 3ULL, 7ULL, 31ULL, 127ULL };
    enum { STREAM_CNT = 4 };

    struct Stream streams[STREAM_CNT];
    seq_t curr_val[STREAM_CNT];

    for (size_t i = 0; i < STREAM_CNT; ++i) {
        streams[i].base   = perfect_N[i];
        streams[i].p_idx  = lower_bound(primes, primes_cnt, threshold_q[i]);
        streams[i].used_two = false;
        curr_val[i] = streams[i].base * (seq_t)2;   /* 2·N */
    }

    /* sentinel for exhausted streams – large enough for all practical uses */
    const seq_t SENTINEL = (seq_t)UINT64_MAX;

    size_t count = 0;
    for (;;) {
        /* find minimal current value among all streams */
        size_t min_k = 0;
        for (size_t i = 1; i < STREAM_CNT; ++i)
            if (curr_val[i] < curr_val[min_k])
                min_k = i;

        seq_t value = curr_val[min_k];
        ++count;
        if (count == n)
            return value;

        /* advance the stream that produced the minimal value */
        struct Stream *s = &streams[min_k];
        if (!s->used_two) {
            /* first output from this stream was 2·N */
            s->used_two = true;
            if (s->p_idx < primes_cnt)
                curr_val[min_k] = s->base * (seq_t)primes[s->p_idx];
            else
                curr_val[min_k] = SENTINEL;
        } else {
            ++s->p_idx;
            if (s->p_idx < primes_cnt)
                curr_val[min_k] = s->base * (seq_t)primes[s->p_idx];
            else
                curr_val[min_k] = SENTINEL;
        }
    }
}
