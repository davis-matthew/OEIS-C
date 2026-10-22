/*
 * A338483  a(n) is the smallest number having n smaller numbers with the same
 *          number of divisors.
 * https://oeis.org/A338483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer whose rank in the sequence of
 *            numbers with the same divisor count equals n
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of helper to extend prime table */
static void extend_primes_upto(idx_t limit);

/* Integer square root for arbitrary precision seq_t without overflow. */
static seq_t integer_sqrt(seq_t n)
{
    seq_t low = 0;
    seq_t high = n;
    seq_t ans = 0;
    while (low <= high)
    {
        seq_t mid = (low + high) / 2;
        if (mid == 0)
        {
            ans = 0;
            low = 1;
            continue;
        }
        /* avoid overflow: mid <= n / mid  <=>  mid^2 <= n  */
        if (mid <= n / mid)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return ans;
}

/* Global prime table ---------------------------------------------------- */
static idx_t *primes = NULL;     /* array of primes (idx_t == uint64_t) */
static idx_t primes_cnt = 0;     /* number of primes stored          */
static idx_t primes_limit = 0;   /* maximum value that primes covers */

/* Simple sieve that rebuilds the prime table up to 'limit'.  */
static void extend_primes_upto(idx_t limit)
{
    if (limit <= primes_limit)
        return;

    /* Allocate a boolean array for the sieve */
    bool *is_composite = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_composite)
        return;                     /* allocation failure – unlikely in tests */

    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (!is_composite[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    /* Count primes to reallocate array */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++new_cnt;

    idx_t *new_primes = (idx_t*)malloc(new_cnt * sizeof(idx_t));
    if (!new_primes)
    {
        free(is_composite);
        return;
    }

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            new_primes[k++] = i;

    free(is_composite);

    /* Replace old table */
    if (primes)
        free(primes);
    primes = new_primes;
    primes_cnt = (idx_t)new_cnt;
    primes_limit = limit;
}

/* Divisor count of a seq_t integer using the global prime table */
static idx_t divisor_count(seq_t n)
{
    if (n <= 1)
        return 1;      /* tau(1) = 1 */

    /* Determine sqrt(n) to bound the trial division */
    seq_t sqrt_seq = integer_sqrt(n);
    idx_t sqrt_lim = (idx_t)sqrt_seq;

    /* Ensure we have primes up to sqrt(n) */
    extend_primes_upto(sqrt_lim * 2 + 1);   /* a bit of slop */

    idx_t cnt = 1;
    seq_t tmp = n;
    for (idx_t i = 0; i < primes_cnt; ++i)
    {
        idx_t p = primes[i];
        if ((seq_t)p * (seq_t)p > tmp)
            break;
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }
    if (tmp > 1)      /* remaining prime factor > sqrt(n) */
        cnt *= 2;
    return cnt;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static seq_t *best = NULL;          /* best[1..]  (best[0] unused) */
    static idx_t best_len = 0;          /* current allocated length   */
    static idx_t *cnt = NULL;           /* cnt[d] = seen numbers with divisor count d   */
    static idx_t cnt_len = 0;           /* current allocated length   */
    static idx_t max_requested = 0;
    static idx_t completed = 0;         /* how many indices filled (1..max_requested) */
    static seq_t current_x = 2;         /* next number to examine          */
    static bool initialized = false;    /* one-time prime table build       */

    /* initialize prime table once */
    if (!initialized)
    {
        extend_primes_upto(2000000);  /* about 148933 primes – plenty for sqrt(n) */
        initialized = true;
    }

    /* enlarge best array if necessary */
    if (n + 1 > best_len)
    {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_best = (seq_t*)realloc(best, new_len * sizeof(seq_t));
        if (!new_best)
            return 0;               /* allocation failure – unlikely */
        for (size_t i = best_len; i < new_len; ++i)
            new_best[i] = 0;
        best = new_best;
        best_len = (idx_t)new_len;
    }

    if (n > max_requested)
        max_requested = n;

    /* iterate numbers until all required a(i) are known */
    while (completed < max_requested)
    {
        /* divisor count of current_x */
        idx_t d = divisor_count(current_x);

        if (d >= cnt_len)
        {
            size_t new_len = (size_t)(d + 1);
            idx_t *new_cnt = (idx_t*)realloc(cnt, new_len * sizeof(idx_t));
            if (!new_cnt)
                return 0;           /* allocation failure – unlikely */
            for (size_t i = cnt_len; i < new_len; ++i)
                new_cnt[i] = 0;
            cnt = new_cnt;
            cnt_len = (idx_t)new_len;
        }

        idx_t before = cnt[d];
        if (before >= 1 && before <= max_requested && best[before] == 0)
        {
            best[before] = current_x;
            ++completed;
        }
        ++cnt[d];
        ++current_x;
    }

    return best[n];
}
