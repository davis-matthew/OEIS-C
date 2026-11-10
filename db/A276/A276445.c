/*
 * A276445  Permutation of natural numbers: a(1) = 1, a(A087686(n)) =
 *          A001969(1+a(n-1)), a(A088359(n)) = A000069(1+a(n)), where A088359
 *          & A087686 = numbers that occur only once & more than once in
 *          A004001, and A000069 & A001969 are odious & evil numbers.
 * https://oeis.org/A276445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(1) = 1; for n > 1, if n occurs more than once in A004001 then
 *            a(n) = A001969(1+a(n-A004001(n))) (rank of n among such numbers
 *            = n-A004001(n)+1), else a(n) = A000069(1+a(A004001(n)-1))
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 98 terms through n = 6886 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *hc_arr = NULL;
static uint64_t hc_len = 0;   /* valid indices 1..hc_len-1 */
static uint64_t hc_cap = 0;

static void hc_ensure(uint64_t k)
{
    if (hc_cap == 0) {
        hc_cap = 64;
        hc_arr = (uint64_t *)malloc(hc_cap * sizeof(uint64_t));
        hc_arr[0] = 0; hc_arr[1] = 1; hc_arr[2] = 1;
        hc_len = 3;
    }
    if (k >= hc_cap) {
        while (k >= hc_cap) hc_cap *= 2;
        hc_arr = (uint64_t *)realloc(hc_arr, hc_cap * sizeof(uint64_t));
    }
    while (hc_len <= k) {
        uint64_t i = hc_len;
        uint64_t p = hc_arr[i - 1];
        hc_arr[i] = hc_arr[p] + hc_arr[i - p];
        hc_len++;
    }
}

static uint64_t hc_get(uint64_t k)
{
    hc_ensure(k);
    return hc_arr[k];
}

static int par_seq(seq_t x)
{
    int p = 0;
    while (x != 0) {
        p ^= (int)(x & 1);
        x >>= 1;
    }
    return p;
}

/* evil number: A001969(k) = 2k - (popcount(k-1) odd ? 1 : 2) */
static seq_t A001969_a(seq_t k)
{
    return k * 2 - (par_seq(k - 1) ? 1 : 2);
}

/* odious number: A000069(k) = 2(k-1) + [popcount(k-1) even] */
static seq_t A000069_a(seq_t k)
{
    seq_t m = k - 1;
    seq_t r = m * 2;
    if (!par_seq(m)) r += 1;
    return r;
}

static seq_t A276445_f(uint64_t n)
{
    if (n <= 1) return (seq_t)1;
    uint64_t cn = hc_get(n);
    uint64_t cnt = 0;
    for (uint64_t k = 1; ; k++) {
        uint64_t v = hc_get(k);
        if (v == n) cnt++;
        else if (v > n) break;
    }
    if (cnt > 1) {
        uint64_t r = n - (cn - 1);
        seq_t val = A276445_f(r - 1);
        return A001969_a(val + 1);
    } else {
        uint64_t m = cn - 1;
        seq_t val = A276445_f(m);
        return A000069_a(val + 1);
    }
}

seq_t a(idx_t n)
{
    return A276445_f((uint64_t)n);
}
