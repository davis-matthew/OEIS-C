/*
 * A321510  Primes p for which there exists a prime q < p such that 3*q == 1
 *          (mod p).
 * https://oeis.org/A321510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A321510 – primes p for which there exists a prime q < p
 *  satisfying 3*q ≡ 1 (mod p).
 *
 *  For any prime p>3, the unique integer q with 3*q ≡ 1 (mod p) is
 *      q = (2*p + 1)/3   if p ≡ 1 (mod 3)
 *      q = (p + 1)/3     if p ≡ 2 (mod 3)
 *
 *  p is a term of the sequence iff q is prime and q < p.
 *
 *  The function listed below computes the n‑th term on demand by
 *  generating primes sequentially with `next_prime` and checking the
 *  above condition.  Results are stored in a static array so that each
 *  term is computed only once per instance.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"        /* next_prime(uint64_t) */
#include "../utils/is_prime.h"          /* is_prime(idx_t) */

static seq_t *tree_terms   = NULL;      /* array of found terms   */
static size_t tree_count   = 0;          /* number of terms cached */
static size_t tree_cap     = 0;          /* capacity of array      */
static idx_t  last_prime   = 2;          /* last prime processed   */

/* Ensure the array can hold at least 'need' elements. */
static void ensure_terms_cap(size_t need)
{
    if (need <= tree_cap) return;
    size_t new_cap = tree_cap ? tree_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    tree_terms = (seq_t *)realloc(tree_terms, new_cap * sizeof(seq_t));
    tree_cap   = new_cap;
}

/* Generate terms until at least 'target' terms have been found. */
static void generate_terms(size_t target)
{
    while (tree_count < target) {
        idx_t p = next_prime(last_prime);   /* next prime > last_prime */
        last_prime = p;

        if (p <= 3) continue;               /* p = 3 has no inverse of 3 */

        idx_t q;
        if (p % 3 == 1) {
            /* q = (2p+1)/3  – use __int128 to avoid overflow */
            q = (idx_t)(((__int128)2 * p + 1) / 3);
        } else { /* p % 3 == 2 */
            q = (p + 1) / 3;
        }

        if (q >= p)       continue;          /* need q < p */
        if (!is_prime(q)) continue;          /* q must be prime */

        ensure_terms_cap(tree_count + 1);
        tree_terms[tree_count++] = (seq_t)p;
    }
}

/* Public sequence function.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;     /* undefined for n=0 */
    generate_terms((size_t)n);
    return tree_terms[(size_t)n - 1];
}
