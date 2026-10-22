/*
 * A377286  Numbers k such that there are no prime-powers between prime(k)+1
 *          and prime(k+1)-1.
 * https://oeis.org/A377286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A377286:  numbers k such that no prime‑powers lie strictly
    between prime(k)+1 and prime(k+1)-1.
    a(n) returns the n‑th such integer k (1‑based indexing). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- internal prime storage ----------------------------------------- */
static uint32_t *prime_list = NULL;    /* list of primes (ascending) */
static size_t prime_cnt   = 0;         /* number of primes stored */
static size_t prime_cap   = 0;         /* current capacity of prime_list */
static uint32_t next_candidate = 2;    /* next integer to test for primality */

/* --- internal sequence cache ---------------------------------------- */
static seq_t  *seq_cache  = NULL;  /* found k values */
static size_t  seq_len    = 0;     /* number of k’s stored */
static size_t  seq_cap    = 0;     /* capacity of seq_cache */

/* --- bookkeeping for processed k ------------------------------------ */
static idx_t last_k_processed = 0;  /* last k checked (1‑based) */

/* --------------------------------------------------------------------- */
static void grow_array(void **arr, size_t *cap, size_t new_needed, size_t elem_size)
{
    if (*cap >= new_needed) return;
    size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
    while (newcap < new_needed) newcap <<= 1;
    void *p = realloc(*arr, newcap * elem_size);
    if (!p) abort();              /* out of memory */
    *arr = p; *cap = newcap;
}

/* --------------------------------------------------------------------- */
static bool is_prime_uint32(uint32_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
static void ensure_primes(size_t needed)
{
    if (prime_cnt >= needed) return;
    while (prime_cnt < needed) {
        while (!is_prime_uint32(next_candidate)) ++next_candidate;
        /* add prime */
        grow_array((void**)&prime_list, &prime_cap, prime_cnt + 1, sizeof(uint32_t));
        prime_list[prime_cnt++] = next_candidate;
        ++next_candidate;
    }
}

/* --------------------------------------------------------------------- */
static bool interval_has_prime_power(uint32_t low, uint32_t high)
{
    if (low > high) return false;      /* empty interval */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t val = (uint64_t)p * p;    /* p^2 */
        if (val > high) break;            /* further primes too large */
        while (val <= high) {
            if (val >= low) return true;  /* prime power found */
            if (val > ((uint64_t)high) / p) break;
            val *= p;                     /* next power of p */
        }
    }
    return false;
}

/* --------------------------------------------------------------------- */
static void extend_to_n(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* next index k to test (1‑based) */
        idx_t k = last_k_processed + 1;

        /* ensure we have primes up to k+1 (indices 0‑based => k+1 entries) */
        ensure_primes((size_t)(k + 1));

        uint32_t p_k   = prime_list[k - 1];
        uint32_t p_next = prime_list[k];
        uint32_t low  = p_k   + 1;
        uint32_t high = p_next - 1;

        if (!interval_has_prime_power(low, high)) {
            /* k qualifies; store it */
            grow_array((void**)&seq_cache, &seq_cap, seq_len + 1,
                       sizeof(seq_t));
            seq_cache[seq_len++] = (seq_t)k;
        }

        last_k_processed = k;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1; undefined for n=0 */
    extend_to_n(n);
    return seq_cache[n - 1];
}
