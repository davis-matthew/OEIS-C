/*
 * A361021  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A007814(i) = A007814(j), A001065(i) = A001065(j) and A051953(i)
 *          = A051953(j), for all i, j >= 1.
 * https://oeis.org/A361021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [A007814(n),
 *            A001065(n), A051953(n)]; computed by scanning n upward and
 *            numbering each new triplet in order of first appearance
 * Range:     u8 n <= 343, u16 n <= 82237, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 114 terms through n = 9717 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t A361021_val2(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            if (i < n) sum += (seq_t)i;
            idx_t c = n / i;
            if (c < n && c != i) sum += (seq_t)c;
        }
    }
    return sum;
}

static seq_t A361021_phi(idx_t m) {
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

static idx_t A361021_done = 0;      /* terms computed so far */
static idx_t A361021_cap = 0;       /* capacity of ans */
static uint64_t *A361021_ans = NULL;
static idx_t A361021_ndist = 0;     /* distinct triplets */
static idx_t A361021_dcap = 0;
static seq_t *A361021_t1 = NULL;
static seq_t *A361021_t2 = NULL;
static seq_t *A361021_t3 = NULL;

static void A361021_grow_dist(void) {
    idx_t nc = A361021_dcap ? A361021_dcap * 2 : 1024;
    seq_t *n1 = mem_new(seq_t, nc);
    seq_t *n2 = mem_new(seq_t, nc);
    seq_t *n3 = mem_new(seq_t, nc);
    for (idx_t i = 0; i < A361021_ndist; i++) {
        n1[i] = A361021_t1[i];
        n2[i] = A361021_t2[i];
        n3[i] = A361021_t3[i];
    }
    if (A361021_t1) { mem_free(A361021_t1); mem_free(A361021_t2); mem_free(A361021_t3); }
    A361021_t1 = n1; A361021_t2 = n2; A361021_t3 = n3;
    A361021_dcap = nc;
}

seq_t a(idx_t n) {
    if (n > A361021_done) {
        if (n > A361021_cap) {
            idx_t nc = n * 2 > 1024 ? n * 2 : 1024;
            uint64_t *na = (uint64_t *)malloc((size_t)nc * sizeof(uint64_t));
            for (idx_t i = 0; i < A361021_done; i++) na[i] = A361021_ans[i];
            free(A361021_ans);
            A361021_ans = na;
            A361021_cap = nc;
        }
        for (idx_t k = A361021_done + 1; k <= n; k++) {
            seq_t v1 = 0;
            idx_t m = k;
            while ((m & 1) == 0) { v1++; m >>= 1; }
            seq_t v2 = A361021_val2(k);
            seq_t v3 = (seq_t)k - A361021_phi(k);
            idx_t found = 0;
            for (idx_t j = 0; j < A361021_ndist; j++) {
                if (A361021_t1[j] == v1 && A361021_t2[j] == v2 && A361021_t3[j] == v3) {
                    found = j + 1;
                    break;
                }
            }
            if (!found) {
                if (A361021_ndist == A361021_dcap) A361021_grow_dist();
                A361021_t1[A361021_ndist] = v1;
                A361021_t2[A361021_ndist] = v2;
                A361021_t3[A361021_ndist] = v3;
                A361021_ndist++;
                found = A361021_ndist;
            }
            A361021_ans[k - 1] = (uint64_t)found;
        }
        A361021_done = n;
    }
    return (seq_t)A361021_ans[n - 1];
}
