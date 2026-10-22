/*
 * A347350  Sequence obtained by writing the first 4 integers and skipping 1,
 *          then writing the next 5 integers and skipping 2, then writing the
 *          next 6 and skipping 3, etc.
 * https://oeis.org/A347350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Write blocks of consecutive integers with lengths 4,5,6,… and
 *            skip consecutive integers with lengths 1,2,3,… respectively.
 *            a(n) is the n‑th written integer. For the k‑th block (k=1,2,…)
 *            the block length is L_k = 3 + k, the skip length is S_k = k − 1.
 *            Let W(k) = Σ_{i=1}^{k} L_i = k(k+7)/2 be the t...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the block number k (1‑based) such that W(k) ≥ n */
    idx_t k = 1;
    while ((k * (k + 7)) / 2 < n)
        ++k;

    idx_t i = k - 1;  /* block index, 0‑based */

    /* Number of written integers before this block */
    idx_t written_before = i * (i + 7) / 2;

    /* Starting integer of this block */
    seq_t start = (seq_t)1 + (seq_t)i * ((seq_t)i + 4);

    /* Offset of n within the block (0‑based) */
    seq_t offset = (seq_t)(n - written_before - 1);

    return start + offset;
}
