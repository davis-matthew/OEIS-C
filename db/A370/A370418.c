/*
 * A370418  Triangle read by rows. T(n, k) = (n - k)! * (n + k)!.
 * https://oeis.org/A370418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r - k)! * (r + k)! where r = ⌊(√(8n+1)-1)/2⌋ and k = n -
 *            r(r+1)/2, i.e. a(n) is the (n‑th) element read row‑wise from the
 *            triangle T(r,k) = (r-k)!·(r+k)!
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* locate the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ( (idx_t)((r + 1) * (r + 2) / 2) <= n )
        r++;

    idx_t base = r * (r + 1) / 2;  /* first index of row r */
    idx_t k = n - base;            /* column within the row */

    /* factorial cache up to the maximum needed value (r + k <= 2r for n<=10000) */
    static seq_t fact[500];
    static idx_t fact_cap = 0;   /* how many factorials have been computed; fact[0] ready */

    if (fact_cap == 0) {          /* first call: initialise factorial 0 = 1 */
        fact[0] = (seq_t)1;
        fact_cap = 1;
    }

    idx_t need = r + k;
    while (fact_cap <= need) {
        fact[fact_cap] = fact[fact_cap - 1] * (seq_t)fact_cap;
        fact_cap++;
    }

    return fact[r - k] * fact[r + k];
}
