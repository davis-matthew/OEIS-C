/*
 * A307633  RUNS transform of bisection {A307720(2k+1): k>=0}.
 * https://oeis.org/A307633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal values in the bisection
 *            b(k) = A307720(2k+1), k >= 0. A307720 is simulated greedily:
 *            each new term is the smallest x with #{pairs with product
 *            prev*x} < prev*x. A per-prev pointer that only moves forward
 *            makes each step O(1) amortized
 * Range:     u8 n <= 90, u16 n <= 5368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9834 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint32_t *R307633_cnt = NULL;   /* cnt[k] = pairs with product k   */
static size_t R307633_ccap = 0;
static uint64_t *R307633_ptr = NULL;   /* ptr[p] = smallest viable x for p */
static size_t R307633_pcap = 0;

static uint64_t R307633_prev = 0;
static uint64_t R307633_len = 0;
static uint64_t R307633_curval = 0;
static uint64_t R307633_curlen = 0;
static uint64_t *R307633_runs = NULL;
static size_t R307633_nruns = 0;
static size_t R307633_runcap = 0;

static void R307633_ensure_cnt(uint64_t k)
{
    if (k < R307633_ccap) return;
    size_t nc = R307633_ccap ? R307633_ccap : 1024;
    while (nc <= k) nc *= 2;
    R307633_cnt = (uint32_t *)realloc(R307633_cnt, nc * sizeof(uint32_t));
    memset(R307633_cnt + R307633_ccap, 0, (nc - R307633_ccap) * sizeof(uint32_t));
    R307633_ccap = nc;
}

static void R307633_ensure_ptr(uint64_t p)
{
    if (p < R307633_pcap) return;
    size_t nc = R307633_pcap ? R307633_pcap : 1024;
    while (nc <= p) nc *= 2;
    R307633_ptr = (uint64_t *)realloc(R307633_ptr, nc * sizeof(uint64_t));
    for (size_t i = R307633_pcap; i < nc; i++) R307633_ptr[i] = 1;
    R307633_pcap = nc;
}

static void R307633_push(uint64_t v)
{
    if (R307633_nruns >= R307633_runcap) {
        size_t nc = R307633_runcap ? R307633_runcap * 2 : 1024;
        R307633_runs = (uint64_t *)realloc(R307633_runs, nc * sizeof(uint64_t));
        R307633_runcap = nc;
    }
    R307633_runs[R307633_nruns++] = v;
}

static void R307633_step(void)
{
    uint64_t p = R307633_prev;
    R307633_ensure_ptr(p);
    uint64_t x = R307633_ptr[p];
    for (;;) {
        uint64_t prod = p * x;
        R307633_ensure_cnt(prod);
        if ((uint64_t)R307633_cnt[prod] < prod) break;
        x++;
    }
    R307633_ptr[p] = x;          /* all smaller x are permanently full */
    R307633_cnt[p * x]++;
    R307633_prev = x;
    R307633_len++;
    if (R307633_len & 1) {
        if (x == R307633_curval) {
            R307633_curlen++;
        } else {
            R307633_push(R307633_curlen);
            R307633_curval = x;
            R307633_curlen = 1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (R307633_len == 0) {
        R307633_ensure_cnt(16);
        R307633_ensure_ptr(16);
        R307633_prev = 1;
        R307633_len = 1;
        R307633_curval = 1;
        R307633_curlen = 1;
    }
    while ((idx_t)R307633_nruns < n) R307633_step();
    return (seq_t)R307633_runs[n - 1];
}
