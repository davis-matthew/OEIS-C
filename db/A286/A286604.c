/*
 * A286604  a(n) = n mod sum of digits of n in factorial base.
 * https://oeis.org/A286604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n mod sum_of_digits_of_n_in_factorial_base
 * Range:     u8 n <= 120, u16 n <= 9937, u32 n <= 9937, u64 n <= 9937,
 *            u128 n <= 9937, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n modulo the sum of the digits of n in factorial base. */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type for arithmetic. */
    seq_t ns = (seq_t)n;
    /* Find the largest k such that k! <= n. */
    idx_t k = 0;
    seq_t fact = (seq_t)1;          /* fact holds k!; start with 0! */
    while (1) {
        idx_t next_k = k + 1;
        seq_t next_fact = fact * (seq_t)next_k;
        if (next_fact > n)          /* next factorial would be too big */
            break;
        k = next_k;                 /* update k and fact to next factorial */
        fact = next_fact;
    }
    /* Now fact = k! <= n < (k+1)! */
    seq_t sum = (seq_t)0;
    seq_t rem = ns;
    for (idx_t i = k; i > 0; i--) {
        seq_t d = rem / fact;         /* digit for factorial i */
        sum += d;                     /* accumulate digit sum */
        rem -= d * fact;              /* remove contribution */
        fact /= (seq_t)i;             /* move to next lower factorial (i-1)! */
    }
    return ns % sum;                   /* final result */
}
