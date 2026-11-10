/*
 * A276831  For a lesser p=A001359(n-1), n>=2, of twin primes, let B_k be the
 *          sequence defined as A159559 but with initial term k; a(n) is the
 *          smallest m such that B_(p+2)(m)-B_p(m) = max_{t>=2}
 *          (B_(p+2)(t)-B_p(t)).
 * https://oeis.org/A276831
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least t>=2 maximizing B_(p+2)(t)-B_p(t), p=A001359(n-1);
 *            B_k(2)=k, B_k(t)=least m>B_k(t-1) with m prime iff t prime.
 *            Computed by simulating both sequences until they merge
 * Range:     u8 n <= 14, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 69 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *a276831_tw = NULL;
static size_t a276831_cnt = 0;
static uint64_t a276831_lim = 0;

static void a276831_extend(uint64_t newlim) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(newlim + 3));
    uint64_t i, j, top = newlim + 2;
    size_t cap = 1024, c = 0;
    uint64_t *list;
    memset(comp, 0, (size_t)(newlim + 3));
    comp[0] = comp[1] = 1;
    for (i = 2; i <= top / i; i++)
        if (!comp[i])
            for (j = i * i; j <= top; j += i) comp[j] = 1;
    list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 2; i <= newlim; i++) {
        if (!comp[i] && !comp[i + 2]) {
            if (c == cap) {
                cap *= 2;
                list = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
            }
            list[c++] = i;
        }
    }
    free(comp);
    free(a276831_tw);
    a276831_tw = list;
    a276831_cnt = c;
    a276831_lim = newlim;
}

static uint64_t a276831_twin(uint64_t n) {
    while ((uint64_t)a276831_cnt < n) {
        uint64_t nl = a276831_lim ? a276831_lim * 2 : 4096;
        a276831_extend(nl);
    }
    return a276831_tw[n - 1];
}

static bool a276831_isp(uint64_t v) {
    uint64_t d;
    if (v < 2) return false;
    if (v < 4) return true;
    if ((v & 1) == 0) return false;
    for (d = 3; d <= v / d; d += 2)
        if (v % d == 0) return false;
    return true;
}

static uint64_t a276831_next(uint64_t prev, bool needprime) {
    uint64_t m = prev + 1;
    while (a276831_isp(m) != needprime) m++;
    return m;
}

seq_t a(idx_t n) {
    uint64_t p = a276831_twin((uint64_t)n - 1);
    uint64_t x = p, y = p + 2;
    uint64_t t = 2, best = 2, bt = 2;
    while (x != y) {
        t++;
        bool np = a276831_isp(t);
        x = a276831_next(x, np);
        y = a276831_next(y, np);
        if (y > x && y - x > best) {
            best = y - x;
            bt = t;
        }
    }
    return (seq_t)bt;
}
