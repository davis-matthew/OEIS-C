/*
 * A317294  Numbers with a noncomposite number of 1's in their binary
 *          expansion.
 * https://oeis.org/A317294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   sequence A317294 – numbers whose binary popcount is 1 or a
 *            prime. The function returns the n‑th member (n≥1, offset 1)
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: compute popcount of a seq_t value (works for 8/16/32/64/128 bits
   and the arbitrary‑precision type when operator>> is defined). */
static int popcount_seq_t(seq_t x)
{
    int cnt = 0;
    while (x)
    {
        cnt += (int)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* Prime table for values up to 128 (enough for popcount of seq_t). */
static bool prime_tbl[129];
static bool prime_tbl_inited = false;

static void init_prime_tbl(void)
{
    for (int i = 0; i < 129; i++)
        prime_tbl[i] = (i >= 2);
    for (int i = 2; i * i < 129; i++)
        if (prime_tbl[i])
            for (int j = i * i; j < 129; j += i)
                prime_tbl[j] = false;
    prime_tbl_inited = true;
}

static bool popcount_is_noncomposite(int pc)
{
    if (pc == 1) return true;               /* 1 is noncomposite */
    if (!prime_tbl_inited) init_prime_tbl();
    return prime_tbl[pc];
}

/* Memoisation cache: holds the sequence values computed so far. */
static seq_t *cache = NULL;     /* array of values */
static idx_t cache_len = 0;     /* number of stored values */
static size_t cache_cap = 0;    /* allocated capacity */
static seq_t next_num = 1;      /* next integer to test */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but never called */

    /* Compute values until we have at least n of them. */
    while (cache_len < n)
    {
        /* Scan candidate numbers until a noncomposite popcount is found. */
        while (true)
        {
            int pc = popcount_seq_t(next_num);
            if (popcount_is_noncomposite(pc))
                break;
            ++next_num;
        }

        /* Store the found value and advance the candidate counter. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = next_num;
        ++next_num;
    }

    return cache[n - 1];
}
