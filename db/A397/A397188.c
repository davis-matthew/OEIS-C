/*
 * A397188  Antidiagonal sums of the array A397186.
 * https://oeis.org/A397188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A397186(n+1-k, k) = Sum_{k=1..n} ((k+1)^p -
 *            k^p - 1)/p, with p = prime(n+1-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t A397188_pw(seq_t b, uint32_t e)
{
    seq_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

/* true when seq_t is a fixed-width (wrapping) type */
static bool A397188_is_native(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 200; i++) {
        t *= 2;
        if (t == 0) return true;
    }
    return false;
}

/* inverse of odd p modulo 2^width(seq_t), by Newton iteration */
static seq_t A397188_inv(seq_t p)
{
    seq_t x = p;
    int i;
    for (i = 0; i < 8; i++) {
        seq_t t = p * x;
        t = (seq_t)2 - t;
        x = x * t;
    }
    return x;
}

seq_t a(idx_t n)
{
    static int native = -1;
    if (native < 0) native = A397188_is_native() ? 1 : 0;

    uint32_t *pr = (uint32_t *)malloc((size_t)n * sizeof(uint32_t));
    if (!pr) return 0;
    uint32_t c = 1;
    size_t cnt = 0;
    while (cnt < (size_t)n) {
        c++;
        bool ok = true;
        for (size_t i = 0; i < cnt && (uint64_t)pr[i] * pr[i] <= c; i++) {
            if (c % pr[i] == 0) { ok = false; break; }
        }
        if (ok) pr[cnt++] = c;
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; k++) {
        uint32_t p = pr[(size_t)(n - k)]; /* prime(n+1-k) */
        if (p == 2) {
            sum += (seq_t)k; /* ((k+1)^2 - k^2 - 1)/2 = k */
            continue;
        }
        seq_t num = A397188_pw((seq_t)k + 1, p);
        num -= A397188_pw((seq_t)k, p);
        num -= 1;
        if (native) {
            /* exact division by odd p in wrapping arithmetic */
            num *= A397188_inv((seq_t)p);
            sum += num;
        } else {
            sum += num / (seq_t)p;
        }
    }
    free(pr);
    return sum;
}
