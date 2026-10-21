/*
 * A292654  Rank of n when all the numbers h, j*e, k*Pi, for h>=1, j>=1, k>=1,
 *          are jointly ranked.
 * https://oeis.org/A292654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/e) + floor(n/pi)
 * Range:     u8 n <= 2, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Rational upper bounds for e and pi that are close enough for n up to 10000.
   They are chosen as 2718281828460/1000000000000 (≈2.718281828460) and
   314159265359/100000000000 (≈3.14159265359).  They fit into 64‑bit, so they
   are valid for both 64‑bit and arbitrary‑precision builds. */
static const seq_t E_NUM = (seq_t)2718281828460ULL;
static const seq_t E_DEN = (seq_t)1000000000000ULL;   /* 10^12 */
static const seq_t PI_NUM = (seq_t)314159265359ULL;
static const seq_t PI_DEN = (seq_t)100000000000ULL;   /* 10^11 */

/* Compute floor(n / x) for x represented by the fraction num/den,
   where num > x*den (i.e. num/den is an upper bound for x). */
static seq_t floor_div(idx_t n, seq_t num, seq_t den)
{
    /* threshold = n * den  (seq_t multiplication) */
    seq_t threshold = (seq_t)n * den;
    seq_t sum = 0;
    idx_t count = 0;

    /* Incrementally add 'num' until the sum would exceed the threshold */
    while (true) {
        seq_t next = sum + num;
        if (next > threshold)
            break;
        sum = next;
        count++;
    }
    return (seq_t)count;
}

seq_t a(idx_t n)
{
    seq_t cnt_e = floor_div(n, E_NUM, E_DEN);  /* floor(n/e) */
    seq_t cnt_pi = floor_div(n, PI_NUM, PI_DEN); /* floor(n/pi) */
    seq_t result = (seq_t)n + cnt_e + cnt_pi;
    return result;
}
