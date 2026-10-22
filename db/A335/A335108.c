/*
 * A335108  Number of periods of the length-n prefix of the Thue-Morse
 *          sequence (A010060).
 * https://oeis.org/A335108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Brute‑force search for the number of periods of the length‑n
    prefix of the Thue‑Morse sequence (OEIS A335108).
    Complexity: O(n²) time, O(n) space for the prefix bits.
    Works for all idx_t ≥ 1 and any seq_t type (arbitrary‑precision
    integer class or built‑in unsigned integer).  No external
    helpers are used. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Static storage for the Thue‑Morse prefix bits.
   Bits are stored as 0/1 values in successive bytes. */
static unsigned char *tm_seq = (unsigned char*)NULL;
static idx_t tm_len = 0;                  /* number of bits stored */

/* Ensure that tm_seq contains at least n bits of the Thue‑Morse sequence.
   Bits are computed as parity of the index: tm_seq[i] = __builtin_parityll(i).
   This function resizes the array if needed. */
static void ensure_tm(idx_t n)
{
    if (n <= tm_len)
        return;

    /* Allocate a new buffer large enough to hold n bits. */
    unsigned char *newp = (unsigned char*)realloc(tm_seq, (size_t)n);
    if (!newp) {
        /* If reallocation fails, terminate the program.  The sequence
           function is not required to handle this error more gracefully
           for the purpose of the OEIS evaluator. */
        exit(EXIT_FAILURE);
    }
    tm_seq = newp;

    /* Compute the new bits from the existing length up to n-1. */
    for (idx_t i = tm_len; i < n; ++i) {
        /* Parity of i gives the Thue‑Morse bit at position i. */
#if defined(__GNUC__) || defined(__clang__)
        tm_seq[i] = (unsigned char)(__builtin_parityll(i));
#else
        /* Portable parity calculation (fallback). */
        idx_t x = i;
        unsigned char p = 0;
        while (x) {
            p ^= 1;
            x &= x - 1;
        }
        tm_seq[i] = p;
#endif
    }
    tm_len = n;
}

/* Number of periods of the length‑n prefix of the Thue‑Morse sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1: values for n ≥ 1 are defined */
    ensure_tm(n);

    seq_t cnt = 0;
    for (idx_t p = 1; p <= n; ++p) {
        idx_t limit = n - p;
        idx_t i;
        for (i = 0; i < limit; ++i) {
            if (tm_seq[i] != tm_seq[i + p])
                break;
        }
        if (i == limit)                 /* all positions matched */
            ++cnt;
    }
    return cnt;
}
