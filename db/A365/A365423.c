/*
 * A365423  Odd numbers k for which k and A163511(k) have the same prime
 *          signature.
 * https://oeis.org/A365423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k such that k and A163511(k) have the same prime
 *            signature (found by search; the exponents of A163511(k) are read
 *            directly from the binary gaps of k)
 * Range:     u8 n <= 13, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void sig_sort_A365423(int *v, int c)
{
    for (int i = 1; i < c; i++) {
        int x = v[i], j = i - 1;
        while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; j--; }
        v[j + 1] = x;
    }
}

static int good_A365423(uint64_t k)
{
    /* exponents of A163511(k) */
    int ex[70];
    int r = 0;
    uint64_t t = k;
    int bitpos = 1, prev = 0;
    while (t) {
        if (t & 1) {
            ex[r++] = bitpos - prev - 1;
            prev = bitpos;
        }
        bitpos++;
        t >>= 1;
    }
    if (r == 0) return 0;
    ex[r - 1]++;
    int sa[70], ca = 0;
    for (int i = 0; i < r; i++) if (ex[i] > 0) sa[ca++] = ex[i];
    sig_sort_A365423(sa, ca);

    /* exponents of k */
    int sb[70], cb = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            sb[cb++] = e;
        }
    }
    if (m > 1) sb[cb++] = 1;
    sig_sort_A365423(sb, cb);

    if (ca != cb) return 0;
    for (int i = 0; i < ca; i++) if (sa[i] != sb[i]) return 0;
    return 1;
}

#define CAP_A365423 200000
static uint64_t terms_A365423[CAP_A365423];
static uint64_t cnt_A365423 = 0;
static uint64_t next_k_A365423 = 1;

seq_t a(idx_t n)
{
    while (cnt_A365423 < n && cnt_A365423 < CAP_A365423) {
        uint64_t k = next_k_A365423;
        next_k_A365423 += 2;
        if (good_A365423(k)) terms_A365423[cnt_A365423++] = k;
    }
    if (n == 0 || n > cnt_A365423) return (seq_t)0;
    return (seq_t)terms_A365423[n - 1];
}
