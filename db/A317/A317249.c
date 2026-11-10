/*
 * A317249  Positions of records in A204911.
 * https://oeis.org/A317249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. a(n) = n-th k where A204911(k) sets a new record.
 *            A204911(k) is computed as follows. Let m = k (k even) or m = 2k
 *            (k odd). For large k it is the least prime q >= 5 with q + m
 *            prime. For small k it is found by scanning primes p and stepping
 *            back by k
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* bit sieve over odd numbers: bit i set <=> 2i+1 composite */
static uint8_t *A317249_bits = NULL;
static uint64_t A317249_lim = 0;

static void A317249_sieve(uint64_t L)
{
    size_t nb = (size_t)(L / 2 + 1);
    size_t bytes = nb / 8 + 1;
    if (A317249_bits) free(A317249_bits);
    A317249_bits = (uint8_t *)calloc(bytes, 1);
    if (!A317249_bits) abort();
    A317249_lim = L;
    A317249_bits[0] |= 1; /* 1 is not prime */
    for (uint64_t i = 3; i * i <= L; i += 2) {
        uint64_t bi = i >> 1;
        if (A317249_bits[bi >> 3] & (1u << (bi & 7))) continue;
        for (uint64_t j = i * i; j <= L; j += 2 * i) {
            uint64_t bj = j >> 1;
            A317249_bits[bj >> 3] |= (uint8_t)(1u << (bj & 7));
        }
    }
}

static bool A317249_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (!(x & 1)) return false;
    if (x > A317249_lim) {
        uint64_t L = A317249_lim * 2;
        if (L < x + 1024) L = x + 1024;
        A317249_sieve(L);
    }
    uint64_t b = x >> 1;
    return !(A317249_bits[b >> 3] & (1u << (b & 7)));
}

/* generic definition: least prime p whose most recent earlier prime
   in the same class mod k is >= 5; returns that earlier prime */
static uint64_t A317249_val_generic(uint64_t k)
{
    for (uint64_t p = 5; ; p++) {
        if (!A317249_isp(p)) continue;
        uint64_t q = p;
        while (q > k) {
            q -= k;
            if (A317249_isp(q)) {
                if (q >= 5) return q;
                break;
            }
        }
    }
}

static uint64_t A317249_val(uint64_t k)
{
    if (k < 2000) return A317249_val_generic(k);
    uint64_t m = (k & 1) ? 2 * k : k;
    for (uint64_t q = 5; q < m; q += 2) {
        if (!A317249_isp(q)) continue;
        if (A317249_isp(q + m)) return q;
    }
    return A317249_val_generic(k); /* never reached in practice */
}

static uint64_t *A317249_rec = NULL;
static size_t A317249_cnt = 0, A317249_cap = 0;
static uint64_t A317249_k = 1, A317249_best = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A317249_bits) A317249_sieve((uint64_t)1 << 20);
    while (A317249_cnt < (size_t)n) {
        uint64_t v = A317249_val(A317249_k);
        if (v > A317249_best) {
            A317249_best = v;
            if (A317249_cnt == A317249_cap) {
                A317249_cap = A317249_cap ? A317249_cap * 2 : 64;
                A317249_rec = (uint64_t *)realloc(A317249_rec, A317249_cap * sizeof(uint64_t));
                if (!A317249_rec) abort();
            }
            A317249_rec[A317249_cnt++] = A317249_k;
        }
        A317249_k++;
    }
    return (seq_t)A317249_rec[(size_t)n - 1];
}
