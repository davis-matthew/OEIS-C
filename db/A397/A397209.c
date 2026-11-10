/*
 * A397209  a(n) is the smallest index k such that A397208(k) = n, or -1 if no
 *          such k exists.
 * https://oeis.org/A397209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A397208(k) = n, where A397208(k) = #{primes
 *            q < prime(k) : (prime(k)+q)/4 is prime}; computed by incremental
 *            sieve search, -1 if not found
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 53 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A397209_MAXV 1024
static uint32_t A397209_first[A397209_MAXV];
static uint8_t *A397209_sv = NULL;
static uint64_t A397209_lim = 0;
static uint64_t A397209_cur = 1;   /* last processed prime */
static uint32_t A397209_k = 0;     /* number of primes processed */

static bool A397209_grow(void)
{
    uint64_t nl = A397209_lim ? A397209_lim * 2 : 1u << 16;
    if (nl > (1ULL << 30)) return false;
    uint8_t *s = (uint8_t *)malloc((size_t)nl + 1);
    if (!s) return false;
    for (uint64_t i = 0; i <= nl; i++) s[i] = 1;
    s[0] = 0; s[1] = 0;
    for (uint64_t i = 2; i * i <= nl; i++)
        if (s[i])
            for (uint64_t j = i * i; j <= nl; j += i) s[j] = 0;
    free(A397209_sv);
    A397209_sv = s;
    A397209_lim = nl;
    return true;
}

seq_t a(idx_t n)
{
    if (n >= A397209_MAXV) return (seq_t)0 - (seq_t)1;
    while (A397209_first[n] == 0) {
        uint64_t p = A397209_cur + 1;
        while (p <= A397209_lim && !A397209_sv[p]) p++;
        if (p > A397209_lim) {
            if (!A397209_grow()) return (seq_t)0 - (seq_t)1;
            continue;
        }
        A397209_cur = p;
        A397209_k++;
        uint64_t cnt = 0;
        for (uint64_t r = 2; 2 * r < p; r++) {
            if (!A397209_sv[r]) continue;
            if (4 * r <= p) continue;
            uint64_t q = 4 * r - p;
            if (q < p && A397209_sv[q]) cnt++;
        }
        if (cnt < A397209_MAXV && A397209_first[cnt] == 0)
            A397209_first[cnt] = A397209_k;
    }
    return (seq_t)A397209_first[n];
}
