/*
 * A246448  Numbers n such that a square will never end in the digits of n.
 * https://oeis.org/A246448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function computes A246448: integers n such that no perfect
 * square ends with the decimal digits of n.
 * Implementation: test whether n is a quadratic residue modulo
 * 10^digits(n).  If not, n belongs to the sequence.
 *
 * The function memoises previously computed terms so that calls
 * in any order are efficient.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool precomp_done = false;
#define MAX_K 10                     /* sufficient for n up to >10^10 */

static uint64_t pow5[MAX_K + 1];
static bool *sq2[MAX_K + 1];
static bool *sq5[MAX_K + 1];

static void precompute(void) {
    if (precomp_done) return;
    pow5[0] = 1ULL;
    for (int k = 1; k <= MAX_K; ++k) pow5[k] = pow5[k - 1] * 5ULL;

    for (int k = 1; k <= MAX_K; ++k) {
        uint64_t mod2 = 1ULL << k;          /* 2^k */
        sq2[k] = (bool*)calloc(mod2, sizeof(bool));
        for (uint64_t i = 0; i < mod2; ++i) {
            uint64_t r = (i * i) & (mod2 - 1);
            sq2[k][r] = true;
        }

        uint64_t mod5 = pow5[k];
        sq5[k] = (bool*)calloc(mod5, sizeof(bool));
        for (uint64_t i = 0; i < mod5; ++i) {
            uint64_t r = (i * i) % mod5;
            sq5[k][r] = true;
        }
    }
    precomp_done = true;
}

/* count decimal digits of a non‑negative integer */
static int digits_uint64(uint64_t x) {
    if (x == 0) return 1;
    int d = 0;
    while (x) { d++; x /= 10ULL; }
    return d;
}

/* test if n is a quadratic residue modulo 10^{digits(n)} */
static bool has_square_suffix(uint64_t n) {
    int k = digits_uint64(n);
    if (k > MAX_K) return false;   /* beyond precomputed range, but never needed */

    uint64_t mod2 = 1ULL << k;
    uint64_t r2 = n & (mod2 - 1);
    if (!sq2[k][r2]) return false;

    uint64_t mod5 = pow5[k];
    uint64_t r5 = n % mod5;
    if (!sq5[k][r5]) return false;

    return true;
}

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t>& seq_vec_ref() {
    static std::vector<seq_t> vec;
    return vec;
}
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; undefined for n<=0 */

    precompute();

#ifdef __cplusplus
    auto &vec = seq_vec_ref();
    static uint64_t cur = 1;
    while (vec.size() < n) {
        uint64_t cand = cur++;
        if (cand == 1) continue;          /* 1 is not part of the sequence */
        if (!has_square_suffix(cand)) {
            vec.push_back((seq_t)cand);
        }
    }
    return vec[n - 1];
#else
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint64_t cur = 1;

    while (seq_len < n) {
        uint64_t cand = cur++;
        if (cand == 1) continue;          /* skip 1 */
        if (!has_square_suffix(cand)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)cand;
        }
    }
    return seq[n - 1];
#endif
}
