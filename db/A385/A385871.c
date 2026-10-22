/*
 * A385871  a(n) is the number of primes in the prime chain to which prime(n)
 *          belongs. Details are in the Comments.
 * https://oeis.org/A385871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes in the prime chain to which prime(n)
 *            belongs (the size of the connected component of the undirected
 *            graph with edges (p, next(p)) for all primes). The
 *            implementation precomputes all primes up to a fixed index,
 *            computes their "next prime" using the definition from OEIS
 *            A385...
 * Range:     u8 none, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Constants (adjustable to guarantee enough primes for all queries) */
#define MAX_IDX 250000          /* number of primes to precompute          */
#define SIEVE_LIMIT 4000000     /* upper bound for prime values (≈ 4 M)   */

/* Global static state ----------------------------------------------------- */
static bool initialized = false;

/* Prime data */
static uint32_t *primes_arr   = NULL;   /* 1‑based index; primes_arr[1] = 2       */
static seq_t   *diff_arr      = NULL;   /* diff[i] = primes_arr[i] - i          */
static idx_t   *next_idx_arr  = NULL;   /* next prime index (0 if none)          */

/* Union‑find data */
static idx_t   *parent_arr    = NULL;
static idx_t   *size_arr      = NULL;

/* Union‑find helpers ------------------------------------------------------- */
static idx_t uf_find(idx_t x)
{
    while (parent_arr[x] != x)
    {
        parent_arr[x] = parent_arr[parent_arr[x]];
        x = parent_arr[x];
    }
    return x;
}

static void uf_union(idx_t a, idx_t b)
{
    idx_t ra = uf_find(a), rb = uf_find(b);
    if (ra == rb) return;
    if (size_arr[ra] < size_arr[rb])
    {
        parent_arr[ra] = rb;
        size_arr[rb] += size_arr[ra];
    }
    else
    {
        parent_arr[rb] = ra;
        size_arr[ra] += size_arr[rb];
    }
}

/* Initialization (runs once) ------------------------------------------------*/
static void init_sequential_chains(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate arrays */
    primes_arr    = (uint32_t *)malloc((MAX_IDX + 1) * sizeof(uint32_t));
    diff_arr      = (seq_t *)malloc((MAX_IDX + 1) * sizeof(seq_t));
    next_idx_arr  = (idx_t *)malloc((MAX_IDX + 1) * sizeof(idx_t));
    parent_arr    = (idx_t *)malloc((MAX_IDX + 1) * sizeof(idx_t));
    size_arr      = (idx_t *)malloc((MAX_IDX + 1) * sizeof(idx_t));

    /* Simple sieve of Eratosthenes (stop once we have MAX_IDX primes) */
    char *is_comp = (char *)calloc(SIEVE_LIMIT + 1, 1);
    idx_t count = 0;
    for (int i = 2; i <= SIEVE_LIMIT && count < MAX_IDX; ++i)
    {
        if (!is_comp[i])
        {
            ++count;
            primes_arr[count] = (uint32_t)i;
            diff_arr[count]   = (seq_t)i - (seq_t)count;  /* prime - index   */
            if ((int64_t)i * i <= SIEVE_LIMIT)
            {
                for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);

    /* Compute next prime index for each prime (if any) */
    for (idx_t i = 1; i <= count; ++i)
    {
        seq_t target = (seq_t)primes_arr[i] + (seq_t)i - 2;
        /* Binary search (lower_bound) for first position where diff >= target */
        idx_t lo = 1, hi = count;
        while (lo < hi)
        {
            idx_t mid = (lo + hi) / 2;
            if (diff_arr[mid] < target)
                lo = mid + 1;
            else
                hi = mid;
        }
        idx_t found = 0;
        if (lo <= count && diff_arr[lo] == target && lo > i)
            found = lo;
        else if (lo <= count && diff_arr[lo] == target && lo <= i)
        {
            /* Duplicate at 1 (only case). Move to the next equal entry. */
            idx_t cand = lo + 1;
            while (cand <= count && diff_arr[cand] == target)
            {
                if (cand > i)
                {
                    found = cand;
                    break;
                }
                ++cand;
            }
        }
        next_idx_arr[i] = found;  /* 0 indicates no next prime */
    }

    /* Initialize union‑find */
    for (idx_t i = 1; i <= count; ++i)
    {
        parent_arr[i] = i;
        size_arr[i]   = 1;
    }

    /* Build undirected components via unions */
    for (idx_t i = 1; i <= count; ++i)
        if (next_idx_arr[i] != 0)
            uf_union(i, next_idx_arr[i]);

    /* parent/size arrays are now ready for queries */
}

/* Public API ---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure precomputation has been done */
    init_sequential_chains();

    /* If n is beyond precomputed range, extend the computation? */
    /* For this problem we assume n <= MAX_IDX. */
    idx_t root = uf_find(n);
    return (seq_t)size_arr[root];
}
