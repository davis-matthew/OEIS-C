/*
 * A364682  Number of iterations of the "x -> sum of squares of digits of x"
 *          map (A003132) for n to converge to either 0, 1 or the 8-cycle
 *          (37,58,89,145,42,20,4,16).
 * https://oeis.org/A364682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of the map * x -> sum of squares of
 *            the decimal digits of x * until the value is 0, 1, or a member
 *            of the * 8‑cycle {37,58,89,145,42,20,4,16}. * The initial number
 *            counts as the first iteration
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_terminal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_terminal.h"

/*  seq_t and idx_t are supplied by the build system.  */
extern seq_t a(idx_t n);

/*  ----- helper: sum of squares of decimal digits for a 64‑bit integer ----- */
static inline uint64_t digit_square_sum_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

/*  ----- memoization table for numbers up to MAXM (maximum sum after one step) ----- */
#define MAXM 2000
static seq_t memo[MAXM];
static bool memo_initialized = false;

/*  ----- recursive computation of the length for numbers < MAXM ----- */
static seq_t compute_len(uint64_t x)
{
    if (x < MAXM && memo[x] != 0)          /* already known */
        return memo[x];

    seq_t result;
    if (x < MAXM && is_terminal(x))        /* terminal */
        result = 1;
    else {
        uint64_t next = digit_square_sum_uint64(x);
        /* next is guaranteed to be < MAXM because MAXM > 1620 */
        result = 1 + compute_len(next);
    }

    if (x < MAXM)
        memo[x] = result;                  /* store for future use */

    return result;
}

/*  ----- one‐time initialization of the memo table ----- */
static void init_memo(void)
{
    for (int i = 0; i < MAXM; ++i)
        memo[i] = 0;
    for (int i = 0; i < MAXM; ++i)
        compute_len((uint64_t)i);
    memo_initialized = true;
}

/*  ----- public function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!memo_initialized)
        init_memo();

    /*  Use memoized result if the number itself is small enough  */
    if (n < MAXM)
        return memo[(uint64_t)n];

    /*  Otherwise perform one step and look up the small result  */
    uint64_t next = digit_square_sum_uint64((uint64_t)n);
    return 1 + memo[next];
}
