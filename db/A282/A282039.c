/*
 * A282039  Let p = n-th prime == 7 mod 8; a(n) = sum of quadratic residues
 *          mod p that are < p/2.
 * https://oeis.org/A282039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 4037, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS Sequence A282039
   Formula: Let pₙ be the n‑th prime congruent to 7 (mod 8). Then 
   a(n) = sum of the quadratic residues modulo pₙ that are < pₙ/2.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *primes7       = NULL;
static size_t primes7_count = 0;
static size_t primes7_cap   = 0;

static void grow_primes7(size_t min_cap)
{
    if (min_cap <= primes7_cap) return;
    size_t new_cap = primes7_cap ? primes7_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) return;            /* allocation failure – caller will crash */
    if (primes7) {
        for (size_t i = 0; i < primes7_count; ++i) tmp[i] = primes7[i];
        free((void *)primes7);
    }
    primes7 = tmp;
    primes7_cap = new_cap;
}

static void ensure_primes7(idx_t n)
{
    if (n <= (idx_t)primes7_count) return;
    /* initial prime 7 */
    if (primes7_count == 0) {
        grow_primes7(1);
        primes7[0] = (seq_t)7;
        primes7_count = 1;
    }
    uint64_t cand = (uint64_t)primes7[primes7_count - 1] + 2; /* next odd */
    while (primes7_count < (size_t)n) {
        if ((cand & 1) && is_prime(cand) && (cand % 8 == 7)) {
            grow_primes7(primes7_count + 1);
            primes7[primes7_count++] = (seq_t)cand;
        }
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_primes7(n);
    seq_t p     = primes7[n - 1];
    seq_t half  = p / 2;
    seq_t sum   = 0;
    seq_t limit = (p - 1) / 2;
    for (idx_t i = 1; i <= (idx_t)limit; ++i) {
        seq_t ii = (seq_t)i;
        seq_t r  = (ii * ii) % p;
        if (r < half) sum += r;
    }
    return sum;
}
