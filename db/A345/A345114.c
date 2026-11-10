/*
 * A345114  Numbers whose trajectories under the map x -> A345111(x) do not
 *          reach a palindrome (conjectured).
 * https://oeis.org/A345114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: k is a term when iterating x -> x + A345110(x) (A345110
 *            = move the leading digit of x to the end, i.e. rotate the digits
 *            left) from x = k reaches no palindrome within 10000 steps; the
 *            start value itself is not checked
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A345114_STEPS 10000
#define A345114_MAXD (A345114_STEPS + 40)
#define A345114_HBITS 18
#define A345114_HSIZE (1u << A345114_HBITS)

static uint64_t A345114_hash[A345114_HSIZE];
static bool A345114_used[A345114_HSIZE];
static uint64_t A345114_hcount = 0;

static bool A345114_hfind(uint64_t v)
{
    uint64_t h = (v * 0x9E3779B97F4A7C15ULL) >> (64 - A345114_HBITS);
    while (A345114_used[h]) {
        if (A345114_hash[h] == v) return true;
        h = (h + 1) & (A345114_HSIZE - 1);
    }
    return false;
}

static void A345114_hadd(uint64_t v)
{
    uint64_t h;
    if (A345114_hcount >= A345114_HSIZE / 2) return;
    h = (v * 0x9E3779B97F4A7C15ULL) >> (64 - A345114_HBITS);
    while (A345114_used[h]) {
        if (A345114_hash[h] == v) return;
        h = (h + 1) & (A345114_HSIZE - 1);
    }
    A345114_used[h] = true;
    A345114_hash[h] = v;
    A345114_hcount++;
}

static unsigned char A345114_x[A345114_MAXD + 2];
static uint64_t A345114_small[A345114_STEPS + 2];

/* true if the trajectory of k never reaches a palindrome (within the step limit) */
static bool A345114_test(uint64_t k)
{
    unsigned char *x = A345114_x;
    int len = 0;
    uint64_t t = k;
    int nsmall = 0;
    bool result = true;
    int step, i;
    while (t > 0) { x[len++] = (unsigned char)(t % 10); t /= 10; }
    A345114_small[nsmall++] = k;
    for (step = 0; step < A345114_STEPS; step++) {
        /* x + rotl(x): rotl little-endian digit 0 = x[len-1], digit i = x[i-1] */
        int carry = 0;
        int prev = x[len - 1];
        for (i = 0; i < len; i++) {
            int old = x[i];
            int s = old + prev + carry;
            x[i] = (unsigned char)(s % 10);
            carry = s / 10;
            prev = old;
        }
        if (carry) x[len++] = (unsigned char)carry;
        if (len > A345114_MAXD) break;
        {
            int lo = 0, hi = len - 1;
            bool pal = true;
            while (lo < hi) {
                if (x[lo] != x[hi]) { pal = false; break; }
                lo++; hi--;
            }
            if (pal) { result = false; break; }
        }
        if (len <= 18) {
            uint64_t v = 0;
            for (i = len - 1; i >= 0; i--) v = v * 10 + x[i];
            if (A345114_hfind(v)) { result = true; break; }
            A345114_small[nsmall++] = v;
        }
    }
    if (result) {
        for (i = 0; i < nsmall; i++) A345114_hadd(A345114_small[i]);
    }
    return result;
}

#define A345114_CAP 4096
static uint64_t A345114_terms[A345114_CAP];
static idx_t A345114_count = 0;
static uint64_t A345114_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A345114_count < n && A345114_count < A345114_CAP) {
        uint64_t k = A345114_next++;
        if (A345114_test(k)) A345114_terms[A345114_count++] = k;
    }
    if (n > A345114_count) return 0;
    return (seq_t)A345114_terms[n - 1];
}
