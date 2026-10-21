/*
 * A258031  Numbers prime(k) such that D(prime(k), 3) < 0, where D( * , 3) =
 *          3rd difference.
 * https://oeis.org/A258031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A258031: primes p_k such that D(prime(k),3) < 0.
 *  D(p_k,3) = P(k+3) - 3*P(k+2) + 3*P(k+1) - P(k)
 *  (P(m) = prime(m))
 *  The function returns the n-th term (1‑based) of this subsequence.
 *  Prime values are obtained from the helper get_prime().
 *  Complexity: O(n) with negligible constant factor. */

#include "../utils/get_prime.h"

/* ---------- cache for the sequence ---------- */
static seq_t *a_cache = NULL;      /* values of a(n) */
static size_t a_cnt   = 0;          /* number of stored terms */
static size_t a_cap   = 0;          /* current capacity */
static idx_t  next_k  = 1;          /* next prime index to examine */

/* enlarge the a cache to new_cap elements */
static void grow_a_cache(size_t new_cap)
{
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (a_cache) {
        memcpy(new_arr, a_cache, a_cnt * sizeof(seq_t));
        free(a_cache);
    }
    a_cache = new_arr;
    a_cap   = new_cap;
}

/* --------------------------------------------------------------- */
/*  Return the n‑th prime p_k with D(p_k,3) < 0 (n ≥ 1).          */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for non‑positive n */

    while (a_cnt < (size_t)n) {
        /* obtain consecutive primes via helper */
        seq_t p_k  = (seq_t)get_prime(next_k);
        seq_t p_k1 = (seq_t)get_prime(next_k + 1);
        seq_t p_k2 = (seq_t)get_prime(next_k + 2);
        seq_t p_k3 = (seq_t)get_prime(next_k + 3);

        /* left = P(k+3) + 3*P(k+1) */
        seq_t left  = p_k3 + (seq_t)3 * p_k1;
        /* right = 3*P(k+2) + P(k) */
        seq_t right = (seq_t)3 * p_k2 + p_k;

        if (left < right) {              /* D(p_k,3) < 0  */
            if (a_cnt == a_cap)
                grow_a_cache(a_cap ? (a_cap << 1) : 16);
            a_cache[a_cnt++] = p_k;
        }
        next_k++;                         /* advance to next prime index */
    }
    return a_cache[n - 1];
}
