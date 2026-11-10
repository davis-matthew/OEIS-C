/*
 * A390316  Numbers m such that A390254(m) < c*(4/3)^m, where c = lim_{m->oo}
 *          A390254(m)/(4/3)^m = A390321.
 * https://oeis.org/A390316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   m is a term iff sum_{k>=0} e_{m+k}*(3/4)^k > 0, where
 *            A390254(n+1) = (4/3)*A390254(n) + e_n and 3*e_n = 0, -1, +1 for
 *            A390254(n) mod 3 = 0, 1, 2. The sign is decided by exact integer
 *            lookahead, and A390254 residues come from a bignum simulation.
 *            a(n) = n-th such m, counting from m = 0
 * Range:     u8 n <= 152, u16 n <= 9881, u32 n <= 9881, u64 n <= 9881,
 *            u128 n <= 9881, big any n
 * Validated: standard, pass, 99 terms through n = 9355 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *A390316_E = 0;   /* 0 -> E=0, 1 -> E=+1, 2 -> E=-1 */
static size_t A390316_Ecnt = 0, A390316_Ecap = 0;
static uint32_t *A390316_big = 0;
static size_t A390316_len = 0, A390316_bcap = 0;

static size_t *A390316_terms = 0;
static size_t A390316_tcnt = 0, A390316_tcap = 0;
static size_t A390316_next_m = 0;

static void A390316_init(void)
{
    if (A390316_big) return;
    A390316_bcap = 64;
    A390316_big = (uint32_t *)malloc(A390316_bcap * sizeof(uint32_t));
    A390316_big[0] = 2;
    A390316_len = 1;
}

static void A390316_ensure(size_t N)
{
    A390316_init();
    while (A390316_Ecnt < N) {
        /* divide by 3 */
        uint64_t rem = 0;
        for (size_t i = A390316_len; i-- > 0;) {
            uint64_t cur = (rem << 32) | A390316_big[i];
            A390316_big[i] = (uint32_t)(cur / 3);
            rem = cur % 3;
        }
        unsigned r = (unsigned)rem;
        if (A390316_Ecnt >= A390316_Ecap) {
            size_t nc = A390316_Ecap ? A390316_Ecap * 2 : 4096;
            A390316_E = (unsigned char *)realloc(A390316_E, nc);
            A390316_Ecap = nc;
        }
        A390316_E[A390316_Ecnt++] = (unsigned char)(r == 0 ? 0 : (r == 2 ? 1 : 2));
        /* multiply by 4 and add r (+1 if r==2) */
        if (A390316_len + 1 > A390316_bcap) {
            A390316_bcap *= 2;
            A390316_big = (uint32_t *)realloc(A390316_big, A390316_bcap * sizeof(uint32_t));
        }
        uint64_t carry = r + (r == 2 ? 1 : 0);
        for (size_t i = 0; i < A390316_len; ++i) {
            uint64_t cur = ((uint64_t)A390316_big[i] << 2) + carry;
            A390316_big[i] = (uint32_t)cur;
            carry = cur >> 32;
        }
        if (carry) A390316_big[A390316_len++] = (uint32_t)carry;
        while (A390316_len > 1 && A390316_big[A390316_len - 1] == 0) --A390316_len;
    }
}

static int A390316_test(size_t m)
{
    const int KMAX = 58;
    A390316_ensure(m + KMAX + 2);
    __int128 Q = 0;
    __int128 p3 = 1;     /* 3^K */
    for (int K = 0; K < KMAX; ++K) {
        int e = A390316_E[m + K];
        int E = e == 0 ? 0 : (e == 1 ? 1 : -1);
        Q = Q * 4 + (__int128)E * p3 * 4;
        p3 *= 3;
        /* now Q = Q_{K+1}, bound 4*3^(K+1) */
        __int128 bound = 4 * p3;
        if (Q > bound) return 1;
        if (Q < -bound) return 0;
    }
    return Q > 0;
}

seq_t a(idx_t n)
{
    while (A390316_tcnt < (size_t)n) {
        size_t m = A390316_next_m++;
        if (A390316_test(m)) {
            if (A390316_tcnt >= A390316_tcap) {
                size_t nc = A390316_tcap ? A390316_tcap * 2 : 1024;
                A390316_terms = (size_t *)realloc(A390316_terms, nc * sizeof(size_t));
                A390316_tcap = nc;
            }
            A390316_terms[A390316_tcnt++] = m;
        }
    }
    return (seq_t)A390316_terms[(size_t)n - 1];
}
