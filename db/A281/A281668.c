/*
 * A281668  Expansion of Sum_{p prime, i>=1} x^(p^i)/(1 + x^(p^i)) *
 *          Product_{p prime, j>=1} (1 + x^(p^j)).
 * https://oeis.org/A281668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all partitions of n into
 *            distinct prime powers (1 excluded)
 * Range:     u8 n <= 35, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a classic DP over the set of prime powers ≤ n.  It works for any
   integer type seq_t (including arbitrary-precision), with O(n·P) time and O(n) memory
   where P is the number of prime powers ≤ n. */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
#include <new>
#endif

/* Helper: allocate a zero‑initialized array of seq_t, compatible with C and C++ */
static seq_t *alloc_seq_array(size_t n) {
#ifdef __cplusplus
    return new seq_t[n]();     /* value‑initialize each element */
#else
    return (seq_t *)calloc(n, sizeof(seq_t));
#endif
}

/* Helper: free a seq_t array allocated by alloc_seq_array */
static void free_seq_array(seq_t *arr) {
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
}

/* Helper: reallocate a uint32_t array */
static uint32_t *realloc_uint32(uint32_t *ptr, size_t new_cap) {
    return (uint32_t *)realloc(ptr, new_cap * sizeof(uint32_t));
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t N = (size_t)n;          /* workspace size */

    /* DP arrays: number of subsets (cnt) and total parts in those subsets (cntp) */
    seq_t *cnt  = alloc_seq_array(N + 1);
    seq_t *cntp = alloc_seq_array(N + 1);
    if (!cnt || !cntp) {
        free_seq_array(cnt);
        free_seq_array(cntp);
        return 0;                  /* allocation failed */
    }
    cnt[0] = 1;                    /* one empty subset for sum 0 */

    /* Sieve of Eratosthenes to mark primes up to N */
    unsigned char *is_prime = (unsigned char *)calloc(N + 1, sizeof(unsigned char));
    if (!is_prime) { /* allocation failed */
        free_seq_array(cnt); free_seq_array(cntp);
        return 0;
    }
    for (size_t i = 2; i <= N; ++i) is_prime[i] = 1;
    size_t lim = (size_t)sqrt((double)N);
    for (size_t i = 2; i <= lim; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= N; j += i) is_prime[j] = 0;
        }
    }

    /* Gather all prime powers ≤ N into a dynamic array */
    size_t parts_cap = 1024;
    uint32_t *parts = (uint32_t *)malloc(parts_cap * sizeof(uint32_t));
    size_t parts_cnt = 0;
    for (size_t p = 2; p <= N; ++p) {
        if (!is_prime[p]) continue;
        unsigned long long val = p;
        while (val <= N) {
            if (parts_cnt == parts_cap) {
                parts_cap <<= 1;
                parts = realloc_uint32(parts, parts_cap);
                if (!parts) { /* allocation failed */
                    free(is_prime); free_seq_array(cnt); free_seq_array(cntp);
                    return 0;
                }
            }
            parts[parts_cnt++] = (uint32_t)val;
            val *= p;
        }
    }
    free(is_prime);

    /* DP over distinct prime powers */
    for (size_t idx = 0; idx < parts_cnt; ++idx) {
        uint32_t v = parts[idx];
        int iv = (int)v;
        for (int s = (int)N; s >= iv; --s) {
            int prev = s - iv;
            seq_t c = cnt[prev];
            if (c) {
                cntp[s] += cntp[prev] + c;
                cnt[s]  += c;
            }
        }
    }
    free(parts);
    seq_t result = cntp[N];
    free_seq_array(cnt);  free_seq_array(cntp);
    return result;
}
