/*
 * A354321  Digit above the least significant 01 digit pair in the Zeckendorf
 *          representation of n.
 * https://oeis.org/A354321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit above the least significant "01" pair in the
 *            Zeckendorf representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t fib[95];
static int fib_len = 0;
static bool fib_init = false;

/* Initialise the Fibonacci table fib[1] = 1, fib[2] = 2, fib[i] = fib[i-1] + fib[i-2] */
static void init_fib(void)
{
    if (fib_init) return;
    fib[1] = 1;
    fib[2] = 2;
    fib_len = 2;
    /* generate until we would overflow or exceed UINT64_MAX */
    while (true) {
        uint64_t a = fib[fib_len];
        uint64_t b = fib[fib_len - 1];
        if (a > UINT64_MAX - b) break;          /* would overflow */
        fib_len++;
        fib[fib_len] = a + b;
    }
    fib_init = true;
}

/* Return the OEIS sequence A354321 value for the given index n (n >= 1). */
seq_t a(idx_t n)
{
    if (!fib_init) init_fib();

    if (n == 0) return (seq_t)0;   /* safety for out‑of‑range calls */

    uint64_t rem = (uint64_t)n;
    bool bits[95] = {false};

    /* Build the Zeckendorf digits (no consecutive 1s) */
    for (int i = fib_len; i >= 1; --i) {
        if (rem >= fib[i]) {
            bits[i] = true;
            rem -= fib[i];
        }
    }

    /* Find the least significant 01 pair: bit i = 1, bit i+1 = 0 */
    for (int i = 1; i < fib_len; ++i) {
        if (bits[i] && !bits[i + 1]) {
            /* Digit above the pair is bit i+2 if it exists */
            if (i + 2 <= fib_len) return (seq_t)bits[i + 2];
            return (seq_t)0;            /* pair at the topmost position */
        }
    }
    /* No 01 pair found (e.g., for Fibonacci numbers) */
    return (seq_t)0;
}
