/*
 * A325390  Heinz number of the negated differences plus one of the integer
 *          partition with Heinz number n (with the last part taken to be 0).
 * https://oeis.org/A325390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Heinz number of the negated differences plus one of
 *            the integer partition with Heinz number n (with the last part
 *            taken to be 0). The partition is obtained from the prime
 *            factorization of n, mapping each prime factor to its index in
 *            the sequence of primes
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binary_search_idx, build_primes_upto, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/binary_search_idx.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static uint64_t prime_limit = 0;

static void ensure_primes_up_to(uint64_t limit) {
    if (limit <= prime_limit) return;
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

static int cmp_desc(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return 1;
    if (va > vb) return -1;
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;                /* undefined but kept safe */
    if (n == 1) return 1;                /* zero‑factor partition */
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);
    if (pcnt == 0) return 1;             /* n==1 case already handled */

    uint64_t idxs[64];
    size_t ilen = 0;

    /* collect indices corresponding to prime factors */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        ensure_primes_up_to(p);
        size_t pos = binary_search_idx(prime_list, prime_cnt, p);
        if (pos == prime_cnt || prime_list[pos] != p) continue; /* safety */
        uint64_t idx = pos + 1; /* 1‑based index */
        for (uint64_t e = 0; e < exps[i]; ++e) {
            idxs[ilen++] = idx;
        }
    }

    /* sort indices descending */
    if (ilen > 1) qsort(idxs, ilen, sizeof(uint64_t), cmp_desc);

    /* compute Heinz number of negated differences + 1 */
    seq_t result = 1;
    for (size_t i = 0; i < ilen; ++i) {
        uint64_t cur = idxs[i];
        uint64_t nxt = (i + 1 < ilen) ? idxs[i + 1] : 0;
        uint64_t t   = cur - nxt + 1;          /* negated diff + 1 */
        seq_t prime_val = (seq_t)get_prime((idx_t)t);
        result *= prime_val;
    }
    return result;
}
