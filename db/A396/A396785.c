/*
 * A396785  Primes that do not appear at division steps of A381466.
 * https://oeis.org/A396785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: simulate A381466 (a(0)=4; a(i)=a(i-1)+i if
 *            gcd(a(i-1),i)=1, else i/gcd(a(i-1),i)) and record every value
 *            produced at a division step. A prime p is a term when it never
 *            shows up at a division step k*p with k <= 6p-2, so a prime is
 *            settled once step p*(6p-2) has been simulated. Speed-up: af...
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define A396785_GT 4096

static uint8_t  A396785_tc[A396785_GT][3];
static uint32_t A396785_toh[A396785_GT][3];
static uint64_t A396785_tinv[A396785_GT][3][6];
static uint64_t A396785_tlim[A396785_GT][3][6];
static int A396785_tinit = 0;

static int A396785_started = 0;
static uint64_t A396785_m = 0, A396785_v = 0, A396785_g = 0;

static uint64_t *A396785_bits = NULL;
static size_t A396785_nw = 0;
static int A396785_two = 0;

static uint64_t *A396785_res = NULL;
static size_t A396785_cnt = 0, A396785_cap = 0;
static uint64_t A396785_cand = 1;

static uint64_t A396785_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void A396785_init_table(void)
{
    for (uint64_t g = 1; g < A396785_GT; g++) {
        for (int jj = 0; jj < 3; jj++) {
            uint64_t j = (uint64_t)jj + 2, t = j * (j - 1) / 2;
            uint64_t h = g * t + j;
            while ((h & 1) == 0) h >>= 1;
            A396785_toh[g][jj] = (uint32_t)h;
            int c = 0;
            uint64_t r = h;
            for (uint64_t q = 3; q * q <= r; q += 2) {
                if (r % q == 0) {
                    uint64_t x = q;
                    for (int it = 0; it < 6; it++) x *= 2 - q * x;
                    A396785_tinv[g][jj][c] = x;
                    A396785_tlim[g][jj][c] = UINT64_MAX / q;
                    c++;
                    while (r % q == 0) r /= q;
                }
            }
            if (r > 1) {
                uint64_t q = r, x = q;
                for (int it = 0; it < 6; it++) x *= 2 - q * x;
                A396785_tinv[g][jj][c] = x;
                A396785_tlim[g][jj][c] = UINT64_MAX / q;
                c++;
            }
            A396785_tc[g][jj] = (uint8_t)c;
        }
    }
    A396785_tinit = 1;
}

static void A396785_mark(uint64_t v)
{
    if (v == 2) { A396785_two = 1; return; }
    if ((v & 1) == 0) return;
    uint64_t idx = v >> 1;
    size_t w = (size_t)(idx >> 6);
    if (w >= A396785_nw) {
        size_t nw = A396785_nw ? A396785_nw : 4096;
        while (nw <= w) nw *= 2;
        uint64_t *tmp = (uint64_t *)realloc(A396785_bits, nw * sizeof(uint64_t));
        if (!tmp) abort();
        memset(tmp + A396785_nw, 0, (nw - A396785_nw) * sizeof(uint64_t));
        A396785_bits = tmp;
        A396785_nw = nw;
    }
    A396785_bits[w] |= (uint64_t)1 << (idx & 63);
}

static int A396785_marked(uint64_t p)
{
    if (p == 2) return A396785_two;
    uint64_t idx = p >> 1;
    size_t w = (size_t)(idx >> 6);
    if (w >= A396785_nw) return 0;
    return (int)((A396785_bits[w] >> (idx & 63)) & 1);
}

static void A396785_run_to(uint64_t T)
{
    if (!A396785_tinit) A396785_init_table();
    if (!A396785_started) {
        uint64_t prev = 4, cur = 0;
        for (;;) {
            uint64_t i = cur + 1, gg = A396785_gcd(prev, i);
            if (gg == 1) { prev += i; cur = i; }
            else {
                A396785_v = i / gg; A396785_g = gg; A396785_m = i;
                A396785_mark(A396785_v);
                A396785_started = 1;
                break;
            }
        }
    }
    uint64_t m = A396785_m, v = A396785_v, g = A396785_g;
    while (m < T) {
        uint64_t prev = v;
        for (uint64_t j = 1;; j++) {
            uint64_t i = m + j, go = 1;
            if (j > 1) {
                uint64_t t = j * (j - 1) / 2;
                if (j <= 4 && g < A396785_GT && v > t) {
                    uint64_t w = v - t;
                    int jj = (int)j - 2;
                    int c = A396785_tc[g][jj];
                    for (int k = 0; k < c; k++) {
                        if (w * A396785_tinv[g][jj][k] <= A396785_tlim[g][jj][k]) {
                            go = A396785_gcd(w, (uint64_t)A396785_toh[g][jj]);
                            break;
                        }
                    }
                } else {
                    go = A396785_gcd(prev, i);
                    while ((go & 1) == 0) go >>= 1;
                }
            }
            unsigned s = 0;
            if (((i | prev) & 1) == 0) {
                unsigned a1 = (unsigned)__builtin_ctzll(prev);
                unsigned b1 = (unsigned)__builtin_ctzll(i);
                s = a1 < b1 ? a1 : b1;
            }
            if (go == 1 && s == 0) { prev += i; continue; }
            v = (i >> s) / go;
            g = go << s;
            m = i;
            A396785_mark(v);
            break;
        }
    }
    A396785_m = m; A396785_v = v; A396785_g = g;
}

static int A396785_isprime(uint64_t x)
{
    if (x < 2) return 0;
    if (x < 4) return 1;
    if ((x & 1) == 0) return 0;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (A396785_cnt < (size_t)n) {
        uint64_t p = A396785_cand + 1;
        while (!A396785_isprime(p)) p++;
        A396785_cand = p;
        A396785_run_to(p * (6 * p - 2));
        if (!A396785_marked(p)) {
            if (A396785_cnt == A396785_cap) {
                size_t nc = A396785_cap ? A396785_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(A396785_res, nc * sizeof(uint64_t));
                if (!tmp) abort();
                A396785_res = tmp;
                A396785_cap = nc;
            }
            A396785_res[A396785_cnt++] = p;
        }
    }
    return (seq_t)A396785_res[n - 1];
}
