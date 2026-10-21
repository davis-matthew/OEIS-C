/*
 * A290444  a(n) = Bell(n)*Motzkin(n).
 * https://oeis.org/A290444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n) * Motzkin(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0)=1, a(1)=1, a(2)=4, a(3)=20, a(4)=135, ... */

#include <stdlib.h>
#include <stdint.h>

/* Static caches for Bell and Motzkin numbers. */
static seq_t *bell_cache = NULL;
static size_t  bell_cap = 0;
static idx_t   bell_len  = 0;          /* computed values for indices 0 .. bell_len-1 */

static seq_t *mot_cache = NULL;
static size_t  mot_cap  = 0;
static idx_t   mot_len  = 0;          /* computed values for indices 0 .. mot_len-1 */

/* Ensure Bell numbers up to index n are computed. */
static void ensure_bell(idx_t n)
{
    if (bell_len > n)
        return;

    /* Allocate or grow the cache. */
    idx_t needed = n + 1;
    if (bell_cap < (size_t)needed)
    {
        size_t new_cap = bell_cap ? bell_cap * 2 : 1;
        if (new_cap < (size_t)needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(bell_cache, new_cap * sizeof(seq_t));
        bell_cache = tmp;
        bell_cap = new_cap;
    }

    /* Initialise first value if cache was empty. */
    if (bell_len == 0)
    {
        bell_cache[0] = (seq_t)1;
        bell_len = 1;
    }

    /* Compute Bell numbers from current length up to n. */
    for (idx_t i = bell_len; i <= n; ++i)
    {
        idx_t N = i - 1;                    /* B(i)=sum_{k=0}^{i-1} C(i-1,k)*B(k) */
        seq_t sum = (seq_t)0;
        seq_t comb = (seq_t)1;              /* C(N,0) */
        for (idx_t k = 0; k <= N; ++k)
        {
            sum += comb * bell_cache[k];
            if (k < N)
            {
                /* Update binomial coefficient: C(N,k+1) = C(N,k)*(N-k)/(k+1) */
                comb = comb * (N - k) / (k + 1);
            }
        }
        bell_cache[i] = sum;
    }
    bell_len = n + 1;
}

/* Ensure Motzkin numbers up to index n are computed. */
static void ensure_mot(idx_t n)
{
    if (mot_len > n)
        return;

    idx_t needed = n + 1;
    if (mot_cap < (size_t)needed)
    {
        size_t new_cap = mot_cap ? mot_cap * 2 : 1;
        if (new_cap < (size_t)needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(mot_cache, new_cap * sizeof(seq_t));
        mot_cache = tmp;
        mot_cap = new_cap;
    }

    /* Initialise first two values if cache was empty. */
    if (mot_len == 0)
    {
        mot_cache[0] = (seq_t)1;
        mot_len = 1;
    }
    if (mot_len == 1)
    {
        if (mot_cap < 2)
        {
            seq_t *tmp = (seq_t *)realloc(mot_cache, 2 * sizeof(seq_t));
            mot_cache = tmp;
            mot_cap = 2;
        }
        mot_cache[1] = (seq_t)1;
        mot_len = 2;
    }

    /* Compute Motzkin numbers from current length up to n. */
    for (idx_t i = mot_len; i <= n; ++i)
    {
        if (i < 2)           /* already handled i=0,1 */
            continue;
        /* Recurrence: M(i) = ((2*i+1)*M(i-1) + (3*i-3)*M(i-2)) / (i+2) */
        seq_t num = (seq_t)(2 * i + 1) * mot_cache[i - 1]
                  + (seq_t)(3 * i - 3) * mot_cache[i - 2];
        mot_cache[i] = num / (i + 2);
    }
    mot_len = n + 1;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_bell(n);
    ensure_mot(n);
    return bell_cache[n] * mot_cache[n];
}
