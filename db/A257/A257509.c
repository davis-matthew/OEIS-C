/*
 * A257509  Numbers n for which A257265(n) = 2; numbers for which the nearest
 *          descendant leaf in the binary beanstalk is two edges away.
 * https://oeis.org/A257509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m >= 0 with A257265(m) = 2, i.e. m has
 *            children under k -> k - popcount(k) (the binary beanstalk), none
 *            of them is a leaf (A055938), but some grandchild is a leaf;
 *            found by search
 * Range:     u8 n <= 32, u16 n <= 8192, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 81 terms through n = 9276 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static int A257509_pop(uint64_t x)
{
    int c = 0;
    while (x) { c += (int)(x & 1U); x >>= 1; }
    return c;
}

/* children of m: x with x - popcount(x) = m, x != m */
static int A257509_kids(uint64_t m, uint64_t *out)
{
    int cnt = 0;
    uint64_t k;
    for (k = 1; k <= 64; k++) {
        uint64_t x = m + k;
        if ((uint64_t)A257509_pop(x) == k) out[cnt++] = x;
    }
    return cnt;
}

static int A257509_is_leaf(uint64_t m)
{
    uint64_t t[64];
    return A257509_kids(m, t) == 0;
}

static int A257509_test(uint64_t n)
{
    uint64_t ch[64], gc[64];
    int c = A257509_kids(n, ch);
    int i, j, g;
    if (c == 0) return 0;
    for (i = 0; i < c; i++)
        if (A257509_is_leaf(ch[i])) return 0;
    for (i = 0; i < c; i++) {
        g = A257509_kids(ch[i], gc);
        for (j = 0; j < g; j++)
            if (A257509_is_leaf(gc[j])) return 1;
    }
    return 0;
}

static uint64_t *A257509_terms = NULL;
static size_t A257509_len = 0, A257509_cap = 0;
static uint64_t A257509_next = 0;

seq_t a(idx_t n)
{
    while (A257509_len < (size_t)n) {
        if (A257509_test(A257509_next)) {
            if (A257509_len == A257509_cap) {
                size_t nc = A257509_cap ? A257509_cap * 2 : 256;
                uint64_t *p = (uint64_t *)realloc(A257509_terms, nc * sizeof(uint64_t));
                if (!p) exit(1);
                A257509_terms = p;
                A257509_cap = nc;
            }
            A257509_terms[A257509_len++] = A257509_next;
        }
        A257509_next++;
    }
    return (seq_t)A257509_terms[(size_t)n - 1];
}
