/*
 * A331546  a(n) = |{k^(k+1)+(k+1)^k (mod prime(n)): k = 0..prime(n)-1}|.
 * https://oeis.org/A331546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k^(k+1) + (k+1)^k (mod prime(n)) : k = 0 .. prime(n)-1
 *            }| where prime(n) is the n‑th prime (prime(1)=2). The sequence
 *            is defined by a set cardinality computed by exhaustive search
 * Range:     u8 n <= 33, u16 n <= 3892, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers – required for all compilers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* External helpers provided by the build system */
#include "../utils/get_prime.h"   /* seq_t get_prime(idx_t k); */

/* ------------------------------------------------------------------ */
/* Helper: fast modular exponentiation for arbitrary seq_t types */
static inline seq_t mod_pow_local(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;          /* handles mod == 1 */
    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}
/* ------------------------------------------------------------------ */
/* Cache for already computed terms.  cache[0] is unused (a(0) is not
   defined).  cache[i] == 0 indicates an uncomputed entry. */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* ------------------------------------------------------------------ */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* below offset – undefined */

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = n + 1;          /* index n must be valid */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {                     /* allocation failure → 0 */
            free(cache); cache = NULL; cache_len = 0;
            return 0;
        }
        /* Zero the new portion of the cache */
        if (new_len > cache_len)
            memset(tmp + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* Return cached value if already computed */
    if (cache[n] != 0) return cache[n];

    /* Compute the n‑th term */
    seq_t prime = get_prime(n);          /* prime(n), type seq_t */
    size_t p = (size_t)prime;            /* p fits in size_t for n <= 10000 */

    /* Allocate a bit map of size p to record seen residues */
    uint8_t *seen = (uint8_t *)calloc(p, 1);
    if (!seen) {                         /* memory error → 0 */
        cache[n] = 0;
        return 0;
    }

    seq_t count = 0;
    for (idx_t k = 0; k < p; ++k) {
        seq_t term1 = mod_pow_local((seq_t)k, k + 1, prime);
        seq_t term2 = mod_pow_local((seq_t)(k + 1), k, prime);

        seq_t sum = term1 + term2;
        if (sum >= prime) sum -= prime;          /* sum mod prime */

        idx_t idx = (idx_t)sum;                  /* 0 <= idx < p */
        if (!seen[idx]) {
            seen[idx] = 1;
            ++count;
        }
    }

    free(seen);
    cache[n] = count;
    return count;
}
