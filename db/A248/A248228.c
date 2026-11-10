/*
 * A248228  Numbers k such that A248227(k+1) = A248227(k).
 * https://oeis.org/A248228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k with A248227(k+1) = A248227(k); found by a search,
 *            tracking A248227 incrementally with a running tail sum of 1/h^4
 * Range:     u8 n <= 79, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Validated: standard, pass, 74 terms through n = 252 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static idx_t *A248228_terms = NULL;
static size_t A248228_cnt = 0, A248228_cap = 0;
static idx_t A248228_m = 0;          /* current index m, prevA = A248227(m) */
static idx_t A248228_prev = 0;
static idx_t A248228_ck = 0;
static long double A248228_tail = 0;
static bool A248228_init = false;

static idx_t A248228_adv(idx_t idx)
{
    long double c = (long double)idx;
    long double thr = 1.0L / (c * c * c);
    while (A248228_tail >= thr) {
        ++A248228_ck;
        long double h = (long double)A248228_ck;
        A248228_tail -= 1.0L / (h * h * h * h);
    }
    return A248228_ck;
}

seq_t a(idx_t n)
{
    if (!A248228_init) {
        A248228_tail = 1.0823232337111381915160036965411679L; /* zeta(4) */
        A248228_cap = 1024;
        A248228_terms = (idx_t *)malloc(A248228_cap * sizeof(idx_t));
        A248228_m = 1;
        A248228_prev = A248228_adv(1);
        A248228_init = true;
    }
    while (A248228_cnt < (size_t)n) {
        idx_t nx = A248228_adv(A248228_m + 1);
        if (nx == A248228_prev) {
            if (A248228_cnt == A248228_cap) {
                A248228_cap *= 2;
                A248228_terms = (idx_t *)realloc(A248228_terms, A248228_cap * sizeof(idx_t));
            }
            A248228_terms[A248228_cnt++] = A248228_m;
        }
        A248228_prev = nx;
        A248228_m++;
    }
    return (seq_t)A248228_terms[n - 1];
}
