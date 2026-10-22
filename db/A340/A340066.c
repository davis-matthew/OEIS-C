/*
 * A340066  Decimal expansion of the Product_{p>=3} 1+p^2/((p-1)^2*(p+1)^2)
 *          where p are successive prime numbers A000040.
 * https://oeis.org/A340066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A340066
 * Decimal expansion of 3465/2764 = 3^2·5·7·11/(2^2·691)
 * a(1) = integer part = 1
 * a(2) onwards = the decimal digits of the periodic expansion
 * The period length is 230.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define NUM_VAL 3465ULL
#define DEN_VAL 2764ULL

/* Compute and cache the period digits once */
static seq_t digit_period[DEN_VAL];
static idx_t period_len = 0;
static seq_t integer_part = 0;
static bool initialized = false;

/* One‑time initialization: find period and store digits */
static void init_period(void)
{
    if (initialized) return;

    /* integer part of the fraction */
    integer_part = (seq_t)(NUM_VAL / DEN_VAL);

    /* remainder after extracting the integer part */
    idx_t rem = NUM_VAL % DEN_VAL;

    /* visited remainders to detect the start of the cycle */
    static int visited[DEN_VAL + 1] = {0};

    /* simulate long division to collect digits until a remainder repeats */
    idx_t step = 0;
    while (visited[rem] == 0) {
        visited[rem] = step + 1;                /* store 1‑based index */
        rem = rem * 10ULL;                       /* bring next decimal digit */
        seq_t d = (seq_t)(rem / DEN_VAL);        /* the digit */
        rem = rem % DEN_VAL;                     /* new remainder */
        digit_period[step] = d;
        ++step;
    }
    period_len = step;
    initialized = true;
}

/* Public function implementing the sequence */
seq_t a(idx_t n)
{
    /* the integer part is a(1) */
    if (n == 1ull) {
        init_period();                /* ensure period is ready for future calls */
        return integer_part;
    }

    init_period();

    /* decimal digits start with a(2) */
    if (period_len == 0) return (seq_t)0;   /* defensive, should not happen */

    idx_t idx = (n - 2ull) % period_len;
    return digit_period[idx];
}
