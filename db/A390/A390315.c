/*
 * A390315  Numbers m such that A390254(m) > c*(4/3)^m, where c = lim_{m->oo}
 *          A390254(m)/(4/3)^m = A390321.
 * https://oeis.org/A390315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   f(0)=2, f(k+1)=round(4*f(k)/3) (A390254); a(n) = n-th m>=1 with
 *            f(m) > c*(4/3)^m, decided exactly via tail sum S_m = sum_{k>=0}
 *            E_{m+k}(3/4)^k < 0, where E = 3*(f(k+1)-4f(k)/3) in {-1,0,1}
 * Range:     u8 n <= 104, u16 n <= 10120, u32 n <= 10120, u64 n <= 10120,
 *            u128 n <= 10120, big any n
 * Validated: standard, pass, 92 terms through n = 7808 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static signed char *oe_E = 0;
static size_t oe_Ecap = 0, oe_Elen = 0;
static uint32_t *oe_f = 0;
static size_t oe_fcap = 0, oe_flen = 0;
static uint32_t *oe_tmp = 0;
static idx_t *oe_terms = 0;
static size_t oe_tcap = 0, oe_tlen = 0;
static idx_t oe_next = 1;

static void oe_extend(size_t want)
{
    if (!oe_f) {
        oe_fcap = 16;
        oe_f = (uint32_t *)malloc(oe_fcap * sizeof(uint32_t));
        oe_tmp = (uint32_t *)malloc(oe_fcap * sizeof(uint32_t));
        oe_f[0] = 2; oe_flen = 1;
    }
    if (want > oe_Ecap) {
        size_t nc = oe_Ecap ? oe_Ecap * 2 : 1024;
        while (nc < want) nc *= 2;
        oe_E = (signed char *)realloc(oe_E, nc);
        oe_Ecap = nc;
    }
    while (oe_Elen < want) {
        size_t i, len = oe_flen;
        if (len + 2 > oe_fcap) {
            oe_fcap *= 2;
            oe_f = (uint32_t *)realloc(oe_f, oe_fcap * sizeof(uint32_t));
            oe_tmp = (uint32_t *)realloc(oe_tmp, oe_fcap * sizeof(uint32_t));
        }
        uint64_t rem = 0;
        for (i = len; i-- > 0;) {
            uint64_t cur = (rem << 32) | oe_f[i];
            oe_tmp[i] = (uint32_t)(cur / 3);
            rem = cur % 3;
        }
        oe_E[oe_Elen++] = (signed char)(rem == 0 ? 0 : (rem == 1 ? -1 : 1));
        uint64_t carry = (rem == 2) ? 1 : 0;
        for (i = 0; i < len; i++) {
            uint64_t s = (uint64_t)oe_f[i] + oe_tmp[i] + carry;
            oe_f[i] = (uint32_t)s;
            carry = s >> 32;
        }
        if (carry) { oe_f[len] = (uint32_t)carry; oe_flen = len + 1; }
    }
}

static int oe_is_term(idx_t m)
{
    long double V = 0.0L;
    size_t k = 0;
    for (;;) {
        size_t idx = (size_t)m + k;
        if (idx >= oe_Elen) oe_extend(idx + 1 + 256);
        V = (4.0L / 3.0L) * (V + (long double)oe_E[idx]);
        k++;
        if (V > 4.0L) return 0;
        if (V < -4.0L) return 1;
        if (k > 100000) return 0;
    }
}

seq_t a(idx_t n)
{
    while (oe_tlen < (size_t)n) {
        if (oe_is_term(oe_next)) {
            if (oe_tlen >= oe_tcap) {
                oe_tcap = oe_tcap ? oe_tcap * 2 : 1024;
                oe_terms = (idx_t *)realloc(oe_terms, oe_tcap * sizeof(idx_t));
            }
            oe_terms[oe_tlen++] = oe_next;
        }
        oe_next++;
    }
    return (seq_t)oe_terms[n - 1];
}
