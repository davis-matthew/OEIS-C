/*
 * A346537  Squares that are divisible by the product of their nonzero digits.
 * https://oeis.org/A346537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A346537: squares divisible by the product of their nonzero
 * digits.  No closed form is known; for a given index n ≥ 1 we scan
 * successive squares m², test the condition, and return the n‑th
 * qualifying square.  The function is thus deterministic for any
 * order of calls and keeps all calculation inside the integer type
 * seq_t. */
 
/* product of the non‑zero decimal digits of x */
static seq_t prod_nonzero_digits(seq_t x)
{
    seq_t prod = 1;
    seq_t t    = x;
    while (t > 0) {
        seq_t d = t % 10;
        if (d > 0)
            prod *= d;
        t /= 10;
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* not defined for offset 0, return 0 safely */
        return 0;

    idx_t count = 0;       /* number of qualifying squares found */
    idx_t root  = 0;       /* current integer whose square is examined */

    while (count < n) {
        ++root;
        seq_t sq   = (seq_t)root * (seq_t)root;
        seq_t prod = prod_nonzero_digits(sq);
        /* prod is always > 0 for sq > 0, so modulo is safe */
        if ((sq % prod) == 0) {
            ++count;
            if (count == n)
                return sq;
        }
    }

    return 0;   /* never reached for valid inputs */
}
