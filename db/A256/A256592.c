/*
 * A256592  Let p = prime(n); a(n) = number of pairs (x,i) with i >= 2 and 2
 *          <= x <= p-i such that x*(x+1)*(x+2)*...*(x+i-1) == 1 mod p.
 * https://oeis.org/A256592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A256592: for the n-th prime p, count pairs (x,i) with 
   i ≥ 2 and 2 ≤ x ≤ p–i such that x⋯(x+i–1) ≡ 1 (mod p). 
   The implementation uses the identity 
     product = fac[x+i‑1] * invfac[x‑1]  (mod p)
   (where fac[k] is k! mod p) and counts equal factorial residues.
   It runs in O(p) time and O(p) memory, where p is the n-th prime. 
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"   /* provides uint64_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* offset starts at 1 */

    /* obtain the n-th prime */
    uint64_t p = get_prime(n);     /* p fits in 64 bits for requested n */
    size_t ps = (size_t)p;         /* use size_t for array indices */

    /* Allocate factorial array [0 .. p-1] */
    uint64_t *fac = (uint64_t *)malloc(ps * sizeof(uint64_t));
    if (!fac)
        return (seq_t)0;           /* allocation failure – unlikely */
    fac[0] = 1;
    for (size_t k = 1; k < ps; ++k)
        fac[k] = (fac[k-1] * (uint64_t)k) % p;

    /* Count adjacent equal factorials (i = 1 cases, not allowed) */
    uint64_t adj = 0;
    for (size_t a = 1; a + 1 < ps; ++a)
        if (fac[a] == fac[a+1])
            adj++;

    /* Frequency table for factorial residues among 1 .. p-1 */
    uint32_t *freq = (uint32_t *)calloc(ps, sizeof(uint32_t));
    if (!freq) {
        free(fac);
        return (seq_t)0;
    }
    for (size_t a = 1; a < ps; ++a)
        ++freq[fac[a]];             /* fac[a] < p, so index is valid */

    /* Sum over residues: choose2(c) pairs, then subtract adjacents */
    uint64_t total = 0;
    for (size_t r = 0; r < ps; ++r) {
        uint32_t c = freq[r];
        if (c > 1)
            total += ((uint64_t)c * (uint64_t)(c-1)) / 2;
    }

    free(freq);
    free(fac);

    uint64_t result = total - adj;
    return (seq_t)result;
}
