/*
 * A334930  Numbers that generate rotationally symmetrical XOR-triangles
 *          featuring singleton zero bits in a hexagonal arrangement.
 * https://oeis.org/A334930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-2) - 8*a(n-4) (n>=5), a(1)=1, a(2)=11, a(3)=13,
 *            a(4)=91
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)91;

    seq_t arr[4];
    arr[0] = (seq_t)1;   /* a(n-4) */
    arr[1] = (seq_t)11;  /* a(n-3) */
    arr[2] = (seq_t)13;  /* a(n-2) */
    arr[3] = (seq_t)91;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)9 * arr[2] - (seq_t)8 * arr[0];
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = next;
    }
    return arr[3];
}
