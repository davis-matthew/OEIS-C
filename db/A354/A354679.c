/*
 * A354679  First differences of A354575.
 * https://oeis.org/A354679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A354575(n+1) - A354575(n); A354575 built by greedy
 *            search: A354575(1)=1, A354575(k) = least unused positive number
 *            coprime to A354575(k-1) whose signed difference is new
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *A354575_v = NULL;
static seq_t *A354575_d = NULL;
static size_t A354575_cap = 0;
static size_t A354575_len = 0;

static void A354575_grow(size_t need)
{
    if (need <= A354575_cap) return;
    size_t nc = A354575_cap ? A354575_cap : 16;
    while (nc < need) nc <<= 1;
    if (A354575_v == NULL) {
        A354575_v = mem_new(seq_t, nc);
        A354575_d = mem_new(seq_t, nc);
    } else {
        A354575_v = mem_resize(seq_t, A354575_v, nc);
        A354575_d = mem_resize(seq_t, A354575_d, nc);
    }
    A354575_cap = nc;
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (A354575_len == 0) {
        A354575_grow(16);
        A354575_v[0] = 1;
        A354575_len = 1;
    }
    A354575_grow(target + 1);
    while (A354575_len < target) {
        seq_t prev = A354575_v[A354575_len - 1];
        seq_t cand = 1;
        while (true) {
            bool bad = false;
            for (size_t i = 0; i < A354575_len; ++i)
                if (A354575_v[i] == cand) { bad = true; break; }
            if (!bad && gcd(prev, cand) != 1) bad = true;
            if (!bad) {
                seq_t d = cand - prev;
                /* differences stored for steps 1..len-1 */
                for (size_t i = 1; i < A354575_len; ++i)
                    if (A354575_d[i] == d) { bad = true; break; }
                if (!bad) {
                    A354575_d[A354575_len] = d;
                    A354575_v[A354575_len] = cand;
                    A354575_len++;
                    break;
                }
            }
            ++cand;
        }
    }
    return A354575_v[n] - A354575_v[n - 1];
}
