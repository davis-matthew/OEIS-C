/*
 * A274923  List of y-coordinates of point moving in counterclockwise square
 *          spiral.
 * https://oeis.org/A274923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -x(n‑1), where x(k) is the x‑coordinate after k steps *
 *            in the counter‑clockwise square spiral that starts at (0,0) with
 *            the * first segment North. The spiral sides have lengths
 *            1,1,2,2,3,3,…. * This implementation walks the spiral
 *            step‑by‑step, yielding an * O(n) algorithm suitable for n...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset 1 (a(1)=0) safely. */
    if (n == 0)
        return 0;

    /* Number of unit‑step moves to reach the desired point. */
    idx_t k = n - 1;

    seq_t x = 0, y = 0;   /* current coordinate */
    int dir = 0;          /* 0:N, 1:W, 2:S, 3:E */
    idx_t stepLen = 1;    /* length of the current side */
    idx_t stepsTaken = 0;
    idx_t sidesDone  = 0; /* tracks two sides before incrementing stepLen */

    while (stepsTaken < k)
    {
        /* Walk along the current side. */
        for (idx_t s = 0; s < stepLen && stepsTaken < k; ++s)
        {
            if (dir == 0)      y += 1;      /* North */
            else if (dir == 1) x -= 1;      /* West */
            else if (dir == 2) y -= 1;      /* South */
            else               x += 1;      /* East */
            ++stepsTaken;
        }

        /* Advance direction and possibly increase side length. */
        dir = (dir + 1) & 3;
        ++sidesDone;
        if (sidesDone == 2)
        {
            sidesDone = 0;
            ++stepLen;
        }
    }

    /* a(n) is the negative of the x‑coordinate of the (n‑1)th step. */
    return -x;
}
