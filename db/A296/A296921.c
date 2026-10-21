/*
 * A296921  Rational primes that decompose in the field Q(sqrt(-163)).
 * https://oeis.org/A296921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 3244, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296921: rational primes that split in Q(√‑163).
   A prime p satisfies Kronecker(−163,p) = 1, equivalently (p|163)=1 and
   p^81 ≡ 1 (mod 163).  The function returns the n‑th such prime
   (a(1) = 41).  It builds the sequence lazily, caching results for
   repeated calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x); */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t *prime_cache = NULL;   /* cached primes */
static size_t   cache_len  = 0;     /* number of cached primes */
static size_t   cache_cap  = 0;     /* allocated capacity */

/* Ensure the cache can hold at least `need` primes. */
static void ensure_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap < 16 ? 16 : cache_cap << 1);
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();            /* out of memory */
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Generate all primes satisfying the splitting condition up to index `n`. */
static void generate_up_to(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    idx_t candidate = (cache_len == 0) ? 2 : (idx_t)(prime_cache[cache_len - 1] + 1);

    while (cache_len < (size_t)n) {
        if (candidate == 2 || (candidate & 1ULL)) {          /* odd candidates + 2 */
            if (is_prime(candidate)) {
                seq_t residue = mod_pow((seq_t)candidate, (uint64_t)81, (seq_t)163);
                if (residue == (seq_t)1) {
                    ensure_cap(cache_len + 1);
                    prime_cache[cache_len++] = (seq_t)candidate;
                }
            }
        }
        ++candidate;
    }
}

/* Return the n-th prime that splits in Q(√‑163). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour for n < 1 is irrelevant */
    generate_up_to(n);
    return prime_cache[n - 1];
}
