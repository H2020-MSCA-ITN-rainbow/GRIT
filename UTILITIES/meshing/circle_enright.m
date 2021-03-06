function circle_enright

addpath('distmesh');

shape = @(p) dcircle(p, 0.5, 0.75, 0.15);

fd = @(p) ddiff(drectangle(p, 0.0, 1.0, 0.0, 1.0), shape(p));
fh = @(p) 0.2+6.0*shape(p);
[p,t] = distmesh2d(fd, fh, 0.02, [0,0;1,1], [0,0;0,1;1,0;1,1  ]);

idxs = abs(shape(p))<1e-3;

hold on;
plot(p(idxs, 1), p(idxs, 2), 'xr')
hold off;

new_fix = p(idxs, :);

figure;
[p_,t_] = distmesh2d(shape,@huniform, 0.03, [0,0;1,1], [new_fix]);

new_t = [t; t_ + size(p,1)];
new_p = [p; p_];

[p,t] = fixmesh(new_p, new_t, 0.005);

px = p(t(:, 1), 1) + p(t(:, 2), 1) + p(t(:, 3), 1);
py = p(t(:, 1), 2) + p(t(:, 2), 2) + p(t(:, 3), 2);

pmid = [px ./ 3, py ./ 3];

close all;

figure;
triplot(t,p(:,1), p(:,2));
axis equal;

hold on;
idxs = shape(pmid) < 1e-4; % idxs = fdInner([p]) < 1e-3;
plot(pmid(idxs, 1), pmid(idxs, 2), '.r') %plot(p(idxs, 1), p(idxs, 2), '.r')
%plot(pmid(:, 1), pmid(:, 2), '.r')
hold off;

export('circle_enright.txt', p, t, idxs);

end
