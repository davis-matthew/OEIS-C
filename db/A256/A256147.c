/*
 * A256147  First repeated number in Sylvester's sequence modulo prime(n).
 * https://oeis.org/A256147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the first repeated residue of Sylvester's sequence modulo the n‑th prime. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* obtain the n-th prime (1‑based) */
    uint32_t p = get_prime(n);
    size_t sz = (size_t)p;

    /* allocate a bitmap of seen residues */
    unsigned char *seen = (unsigned char *)calloc(sz, sizeof *seen);
    if (!seen)
        abort();                 /* out of memory */

    /* start with the first term 2 modulo p, computed using pure integer arithmetic */
    unsigned long long ufirst = 2ULL % (unsigned long long)p;
    seq_t curr = (seq_t)ufirst;
    seen[(size_t)curr] = 1;

    while (1) {
        /* next = curr * (curr - 1) + 1  (mod p) */
        unsigned long long ucurr   = (unsigned long long)curr;
        unsigned long long mod     = (unsigned long long)p;
        unsigned long long sub     = (ucurr + mod - 1ULL) % mod;   /* curr - 1 (mod p) */
        unsigned long long term    = (ucurr * sub + 1ULL) % mod;
        seq_t next = (seq_t)term;

        if (seen[(size_t)next]) {
            free(seen);
            return next;           /* first repeated residue */
        }
        seen[(size_t)next] = 1;
        curr = next;
    }
}
