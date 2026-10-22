/*
 * A333389  Triangle read by rows: T(n,k) is the number of solutions to x^y ==
 *          k (mod n) for n >= 1, k >= 0, where 0 <= x < n and 1 <= y <= n.
 * https://oeis.org/A333389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle of the number of solutions * to 0
 *            ≤ x < r, 1 ≤ y ≤ r with x^y ≡ k (mod r), where * r(r−1)/2 < n ≤
 *            r(r+1)/2 * and k = n − r(r−1)/2 − 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, n ≥ 1 by definition */

    /* Determine the row r for the given flattened index n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Previous total number of terms before row r */
    idx_t prev = (r - 1) * r / 2;
    /* Column k within the row (0‑based) */
    idx_t k = n - prev - 1;

    seq_t count = 0;

    /* Count solutions (x, y) with 0 ≤ x < r, 1 ≤ y ≤ r, x^y ≡ k (mod r) */
    for (idx_t x = 0; x < r; ++x) {
        if (x == 0) {
            if (k == 0) {
                count += (seq_t)r;          /* 0^y == 0 for all y = 1..r */
            }
            continue;
        }
        seq_t power = (seq_t)x;          /* x^1 (mod r) */
        for (idx_t y = 1; y <= r; ++y) {
            if ((seq_t)k == power) {
                ++count;
            }
            power = (power * (seq_t)x) % (seq_t)r;
        }
    }

    return count;
}
