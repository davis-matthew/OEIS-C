/*
 * A285203  Local high points in A285200.
 * https://oeis.org/A285203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = value of the n-th local maximum (strict peak) of A285200,
 *            where A285200 simulates Knowlton's elevator (start at floor 1;
 *            from floor m step up on every m-th visit, else down); computed
 *            by simulation
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A285203_counts = NULL;
static size_t A285203_cap = 0;
static uint64_t A285203_cur = 1;
static uint64_t A285203_prev = 0;
static uint64_t *A285203_peaks = NULL;
static size_t A285203_np = 0;
static size_t A285203_pcap = 0;

static void A285203_ensure_floor(size_t need)
{
    if (need < A285203_cap) return;
    size_t nc = A285203_cap ? A285203_cap : 128;
    while (nc <= need) nc *= 2;
    uint64_t *t = (uint64_t *)realloc(A285203_counts, nc * sizeof(uint64_t));
    memset(t + A285203_cap, 0, (nc - A285203_cap) * sizeof(uint64_t));
    A285203_counts = t;
    A285203_cap = nc;
}

static void A285203_push(uint64_t v)
{
    if (A285203_np == A285203_pcap) {
        size_t nc = A285203_pcap ? A285203_pcap * 2 : 128;
        A285203_peaks = (uint64_t *)realloc(A285203_peaks, nc * sizeof(uint64_t));
        A285203_pcap = nc;
    }
    A285203_peaks[A285203_np++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A285203_counts == NULL) {
        A285203_ensure_floor(128);
        A285203_counts[1] = 1;
        A285203_cur = 1;
        A285203_prev = UINT64_MAX;
    }
    while ((uint64_t)A285203_np < (uint64_t)n) {
        uint64_t cur = A285203_cur;
        bool up = (A285203_counts[cur] % cur) == 0;
        uint64_t nx = up ? cur + 1 : cur - 1;
        A285203_ensure_floor((size_t)nx + 1);
        A285203_counts[nx]++;
        if (cur > A285203_prev && cur > nx) A285203_push(cur);
        A285203_prev = cur;
        A285203_cur = nx;
    }
    return (seq_t)A285203_peaks[n - 1];
}
