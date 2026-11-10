/*
 * A351454  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A006530(i) = A006530(j), A329697(i) = A329697(j) and A331410(i)
 *          = A331410(j) for all i, j >= 1.
 * https://oeis.org/A351454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [A006530(n),
 *            A329697(n), A331410(n)]; computed by scanning 1..n and numbering
 *            distinct triples in order of first appearance
 * Range:     u8 n <= 566, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9889 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A351454_gpf(uint64_t m)
{
    uint64_t g = 1;
    while (m % 2 == 0) { g = 2; m /= 2; }
    for (uint64_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) { g = i; m /= i; }
    }
    if (m > 1) g = m;
    return g;
}

#define A351454_HB 65536u

static uint64_t *A351454_val = 0;   /* a(i) for i = 1..done */
static uint64_t A351454_done = 0, A351454_cap = 0;
static uint64_t *A351454_c1 = 0, *A351454_c2 = 0, *A351454_c3 = 0;
static uint64_t *A351454_nx = 0;
static uint64_t A351454_ncls = 0, A351454_ccap = 0;
static uint64_t A351454_head[A351454_HB];
static bool A351454_init = false;

seq_t a(idx_t n)
{
    if (!A351454_init) {
        for (uint32_t i = 0; i < A351454_HB; i++) A351454_head[i] = 0;
        A351454_init = true;
    }
    if ((uint64_t)n > A351454_done) {
        uint64_t target = (uint64_t)n;
        if (target + 1 > A351454_cap) {
            uint64_t nc = target + 1;
            if (nc < 2 * A351454_cap) nc = 2 * A351454_cap;
            A351454_val = (uint64_t *)realloc(A351454_val, nc * sizeof(uint64_t));
            A351454_cap = nc;
        }
        for (uint64_t i = A351454_done + 1; i <= target; i++) {
            uint64_t g = A351454_gpf(i);
            uint64_t k = i, c2 = 0;
            while ((k & (k - 1)) != 0) {
                uint64_t p = A351454_gpf(k);
                k -= k / p;
                c2++;
            }
            k = i;
            uint64_t c3 = 0;
            while ((k & (k - 1)) != 0) {
                uint64_t p = A351454_gpf(k);
                k += k / p;
                c3++;
            }
            uint64_t h = (g * 1000003ULL + c2 * 10007ULL + c3 * 31ULL) % A351454_HB;
            uint64_t id = A351454_head[h];
            while (id) {
                uint64_t j = id - 1;
                if (A351454_c1[j] == g && A351454_c2[j] == c2 && A351454_c3[j] == c3) break;
                id = A351454_nx[j];
            }
            if (!id) {
                if (A351454_ncls + 1 > A351454_ccap) {
                    uint64_t nc = A351454_ccap ? 2 * A351454_ccap : 1024;
                    A351454_c1 = (uint64_t *)realloc(A351454_c1, nc * sizeof(uint64_t));
                    A351454_c2 = (uint64_t *)realloc(A351454_c2, nc * sizeof(uint64_t));
                    A351454_c3 = (uint64_t *)realloc(A351454_c3, nc * sizeof(uint64_t));
                    A351454_nx = (uint64_t *)realloc(A351454_nx, nc * sizeof(uint64_t));
                    A351454_ccap = nc;
                }
                uint64_t j = A351454_ncls++;
                A351454_c1[j] = g; A351454_c2[j] = c2; A351454_c3[j] = c3;
                A351454_nx[j] = A351454_head[h];
                A351454_head[h] = j + 1;
                id = j + 1;
            }
            A351454_val[i] = id;
        }
        A351454_done = target;
    }
    return (seq_t)A351454_val[(uint64_t)n];
}
