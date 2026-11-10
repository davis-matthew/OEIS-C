/*
 * A335300  a(n) is the least k such that A335299(k) = n, or -1 if no such k
 *          exists.
 * https://oeis.org/A335300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A335299(k) = n, else -1; found by simulating
 *            A335299 (a(0)=0; a(k)=|a(k-1)-k| if new, else a(k-1)+k) and
 *            recording first-occurrence indices, with a step limit
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9964 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A335300_CAP ((unsigned long long)1 << 25)
#define A335300_STEPS 4000000ULL

static uint64_t *A335300_first = NULL;   /* first index + 1, 0 = unseen */
static size_t A335300_size = 0;
static seq_t A335300_cur = 0;
static uint64_t A335300_step = 0;
static bool A335300_init = false;

static bool A335300_ensure(size_t need)
{
    if (need <= A335300_size) return true;
    size_t ns = A335300_size ? A335300_size : 1024;
    while (ns < need) ns *= 2;
    uint64_t *t = (uint64_t *)realloc(A335300_first, ns * sizeof(uint64_t));
    if (!t) return false;
    for (size_t i = A335300_size; i < ns; i++) t[i] = 0;
    A335300_first = t;
    A335300_size = ns;
    return true;
}

static bool A335300_seen(seq_t v)
{
    if (v >= (seq_t)A335300_CAP) return false;
    size_t x = (size_t)(unsigned long long)v;
    return x < A335300_size && A335300_first[x] != 0;
}

static void A335300_mark(seq_t v, uint64_t idx)
{
    if (v >= (seq_t)A335300_CAP) return;
    size_t x = (size_t)(unsigned long long)v;
    if (!A335300_ensure(x + 1)) return;
    if (A335300_first[x] == 0) A335300_first[x] = idx + 1;
}

seq_t a(idx_t n)
{
    if (!A335300_init) {
        A335300_init = true;
        A335300_ensure(1024);
        A335300_cur = 0;
        A335300_step = 0;
        A335300_mark(A335300_cur, 0);
    }
    if (n >= (idx_t)A335300_CAP) return (seq_t)0 - 1;
    size_t t = (size_t)n;
    while ((t >= A335300_size || A335300_first[t] == 0) && A335300_step < A335300_STEPS) {
        A335300_step++;
        seq_t i = (seq_t)A335300_step;
        seq_t prev = A335300_cur;
        seq_t cand = (prev >= i) ? (seq_t)(prev - i) : (seq_t)(i - prev);
        if (A335300_seen(cand)) A335300_cur = prev + i;
        else A335300_cur = cand;
        A335300_mark(A335300_cur, A335300_step);
    }
    if (t < A335300_size && A335300_first[t] != 0)
        return (seq_t)(A335300_first[t] - 1);
    return (seq_t)0 - 1;
}
