/*
 * A392650  Sum-irreducible numbers. A number k > 0 is sum-irreducible if no
 *          subset of its nontrivial divisors {1 < d < k : d | k} can sum to
 *          k.
 * https://oeis.org/A392650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392650: Sum‑irreducible numbers
 * a(n) is the n‑th positive integer k>0 such that no subset of the set
 * { d | 1 < d < k, d divides k } sums to k.
 * Computation is done by scanning integers in order and testing each
 * for sum‑irreducibility using a subset‑sum bitset dynamic program.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- static storage for memoisation ---------- */
static seq_t *seq_mem   = NULL;   /* dynamic array of terms */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* allocated capacity */
static uint64_t next_val = 0;     /* next integer to test */

/* ---------- bitset used for the subset sum test ---------- */
static uint64_t *bitset  = NULL;  /* bitset of reachable sums */
static size_t     bitcap  = 0;     /* number of words allocated */

/* ---------- forward declarations ---------- */
static bool is_sum_irreducible(uint64_t k);
static bool subset_sums_to_k(uint64_t k, const uint64_t *divs, size_t nd);

/* ---------- public function ---------- */
seq_t a(idx_t n) {
    /* initialise memo storage on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq_mem = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        next_val = 0;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        ++next_val; /* candidate integer */
        if (is_sum_irreducible(next_val)) {
            /* store in memoised array */
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_mem = (seq_t *)realloc(seq_mem, seq_cap * sizeof(seq_t));
            }
            seq_mem[seq_len++] = (seq_t)next_val;
        }
    }
    return seq_mem[(size_t)n - 1];
}

/* ---------- helper: test if k is sum‑irreducible ---------- */
static bool is_sum_irreducible(uint64_t k) {
    if (k <= 3) return true;          /* primes and 4 are always irreducible */

    /* factor k: simple trial division */
    uint64_t factors[16];
    size_t   exps[16];
    size_t   pf = 0;
    uint64_t temp = k;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            size_t e = 0;
            do {
                temp /= d;
                ++e;
            } while (temp % d == 0);
            factors[pf] = d;
            exps[pf]    = e;
            ++pf;
        }
    }
    if (temp > 1) {                    /* remaining prime factor */
        factors[pf] = temp;
        exps[pf]    = 1;
        ++pf;
    }

    /* generate all divisors of k */
    uint64_t divs[256];
    size_t   divcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pf; ++i) {
        uint64_t p = factors[i];
        size_t   e = exps[i];
        size_t   curcnt = divcnt;
        for (size_t j = 0; j < curcnt; ++j) {
            uint64_t cur = divs[j];
            for (size_t a = 1; a <= e; ++a) {
                cur *= p;
                divs[divcnt++] = cur;
            }
        }
    }

    /* collect non‑trivial divisors (exclude 1 and k) */
    uint64_t nddivs[256];
    size_t   ndcnt = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        if (d != 1 && d != k) nddivs[ndcnt++] = d;
    }
    if (ndcnt == 0) return true;   /* prime or prime power */

    /* test subset sum */
    return !subset_sums_to_k(k, nddivs, ndcnt);
}

/* ---------- helper: does any subset of divs sum to k? ---------- */
static bool subset_sums_to_k(uint64_t k, const uint64_t *divs, size_t nd) {
    size_t words_needed = (k >> 6) + 1;

    if (words_needed > bitcap) {
        free(bitset);
        bitset = (uint64_t *)malloc(words_needed * sizeof(uint64_t));
        bitcap = words_needed;
    }

    /* clear bitset */
    for (size_t i = 0; i < bitcap; ++i) bitset[i] = 0;
    bitset[0] = 1ULL;  /* sum 0 is reachable */

    const size_t target_word = k >> 6;
    const uint64_t target_mask = 1ULL << (k & 63);

    for (size_t idx = 0; idx < nd; ++idx) {
        uint64_t d = divs[idx];
        const size_t shift_words = d >> 6;
        const uint64_t shift_bits = d & 63;

        /* apply the shift-or DP from high to low indices to avoid reuse */
        int iw = (int)bitcap - 1;
        for (; iw >= (int)shift_words; --iw) {
            uint64_t val = bitset[iw - (int)shift_words] << shift_bits;
            if (shift_bits && iw > (int)shift_words) {
                val |= bitset[iw - (int)shift_words - 1] >> (64 - shift_bits);
            }
            bitset[iw] |= val;
        }

        /* early exit if k becomes reachable */
        if (bitset[target_word] & target_mask) return true;
    }
    return false;   /* no subset sums to k */
}
